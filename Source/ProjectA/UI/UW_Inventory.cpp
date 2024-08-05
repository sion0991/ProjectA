// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Inventory.h"
#include "Gameplay/PC_Player.h"
#include "Character/Player/C_Player.h"
#include "UW_InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/CanvasPanel.h"
#include "Data/ST_Item.h"
#include "Data/InventorySystemComponent.h"
#include "Util/DataFunctionLibrary.h"

UUW_Inventory::UUW_Inventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/*static ConstructorHelpers::FClassFinder<UUW_InventorySlot> FindWidget(TEXT("/Game/Blueprints/UI/CUW_InventorySlot"));
	if (FindWidget.Succeeded())
		SlotWidget = FindWidget.Class;*/
}

void UUW_Inventory::NativeConstruct()
{
	PlayerController = Cast<APC_Player>(GetOwningPlayer());
	Inventory = PlayerController->GetInventorySystemComponent();
	if (!DoOnce)
	{
		for (auto p : CP_Equipment->GetAllChildren())
		{
			UUW_InventorySlot* EquipSlot = Cast<UUW_InventorySlot>(p);
			EquipmentSlots.Add(EquipSlot);
			EquipSlot->DropAtSlot.AddUFunction(this, FName("DropAtEquipSlot"));
		}
		int i = 0;
		for (auto p : GP_InventoryGrid->GetAllChildren())
		{
			InventorySlots.Add(Cast<UUW_InventorySlot>(p));
			InventorySlots[i]->SlotIndex = i;

			InventorySlots[i]->DropAtSlot.AddUFunction(this, FName("DropAtInventorySlot"));
			InventorySlots[i]->DropItem.AddUFunction(this, FName("DropItem"));
			i++;
		}
		DoOnce = true;
	}
	
	LoadInventory();
}

void UUW_Inventory::Init(APC_Player* player)
{
	PlayerController = player;
	for (auto p : CP_Equipment->GetAllChildren())
	{
		EquipmentSlots.Add(Cast<UUW_InventorySlot>(p));
	}
	if (PlayerController)
	{
		for (int i = 0; i < PlayerController->GetInventorySystemComponent()->GetInventoryItems().Num(); i++)
		{
			InventorySlots.Add(Cast<UUW_InventorySlot>(CreateWidget(PlayerController, UUW_InventorySlot::StaticClass())));
			InventorySlots[i]->SlotIndex = i;

			
			UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(GP_InventoryGrid->AddChild(InventorySlots[i]));

			GridSlot->SetRow(i / 5);
			GridSlot->SetColumn(i % 5);

			InventorySlots[i]->DropAtSlot.AddUFunction(this, FName("DropAtInventorySlot"));
			InventorySlots[i]->DropItem.AddUFunction(this, FName("DropItem"));
		}
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UUW_Inventory::LoadInventory()
{
	for (int i = 0; i < InventorySlots.Num(); i++)
	{
		if(FindItemAtSlot(i)->IsValid())
			InventorySlots[i]->PickUpImage = FindItemAtSlot(i)->Icon;
	}
	for (int j = 0; j < EquipmentSlots.Num(); j++)
	{
		if(FindItemAtSlot(j, false)->IsValid())
			EquipmentSlots[j]->PickUpImage = FindItemAtSlot(j, false)->Icon;
	}
	//PlayerController->GetEquipmentItems()
}

void UUW_Inventory::DropAtInventorySlot(UUW_InventorySlot* From, UUW_InventorySlot* To)
{
	From->SlotType == "Inventory" ?
		Inventory->SwapInventoryItem(To->SlotIndex, From->SlotIndex) :
		Cast<AC_Player>(GetOwningPlayerPawn())->TakeOffEquipment(To->SlotIndex, From->SlotIndex);

	SetItem(From->SlotIndex);
	SetItem(To->SlotIndex);
}

void UUW_Inventory::DropItem(UUW_InventorySlot* DropItemSlot)
{
	DragOut ? PlayerController->DropItem(DropItemSlot->SlotIndex):0;
}

void UUW_Inventory::DropAtEquipSlot(UUW_InventorySlot* From, UUW_InventorySlot* To)
{
	if (From->SlotType != "Inventory")
		return;

	//DataFunctionLibrary::GetEnumDisplayNameToName(TEXT("EItemType"), static_cast<uint8>(FindItemAtSlot(From->SlotIndex)->ItemType) == To->SlotType ?
	FName ItemType = DataFunctionLibrary::GetEnumDisplayNameToName(FindItemAtSlot(From->SlotIndex)->ItemType);
	ItemType == To->SlotType ?
		PlayerController->UseItem(From->SlotIndex, To->SlotIndex) :
		ItemType == "TwoHandWeapon" && To->SlotType == "RightWeapon" ?
		PlayerController->UseItem(From->SlotIndex, To->SlotIndex) : 0;

	SwapInvneoryAndEquip(From->SlotIndex, To->SlotIndex);
}

void UUW_Inventory::SetItem(int InventorySlotIndex)
{
	if (IsInViewport())
	InventorySlots[InventorySlotIndex]->PickUpImage = FindItemAtSlot(InventorySlotIndex)->Icon;
	
}

void UUW_Inventory::RemoveItem(int InventorySlotIndex)
{
	InventorySlots[InventorySlotIndex]->PickUpImage = NULL;
}

void UUW_Inventory::SwapInvneoryAndEquip(int InventorySlotIndex, int EquipSlotIndex)
{
	InventorySlots[InventorySlotIndex]->PickUpImage = FindItemAtSlot(InventorySlotIndex)->Icon;
	EquipmentSlots[EquipSlotIndex]->PickUpImage = FindItemAtSlot(EquipSlotIndex, false)->Icon;
}

FST_Item* UUW_Inventory::FindItemAtSlot(int SlotIndex, bool isInventory)
{
	return isInventory ?
		Inventory->GetInventoryItem(SlotIndex) :
		Inventory->GetEquipItem(SlotIndex);
}
