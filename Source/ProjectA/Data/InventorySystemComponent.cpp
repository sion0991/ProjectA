// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"
#include "ST_Item.h"
#include "Gameplay/PlayerHUD.h"
#include "Gameplay/PC_Player.h"
#include "UI/UW_Inventory.h"
#include "SG_PlayerData.h"
#include "Gameplay/PS_PlayerState.h"
#include "Data/Inventory.h"

APS_PlayerState* UInventorySystemComponent::GetPlayerState()
{
	return Cast<APS_PlayerState>(GetOwner());
}

void UInventorySystemComponent::AddItem(const FST_Item& Item)
{
	for (int i = 0; i < GetPlayerState()->GetInventory()->InventoryItems.Num(); i++)
	{
		if (!GetPlayerState()->GetInventory()->InventoryItems[i].IsValid())
		{
			GetPlayerState()->GetInventory()->InventoryItems[i] = Item;

			/*if(InventoryWidget)
				InventoryWidget->SetItem(i);*/
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UInventorySystemComponent Error AddItem"));
}
void UInventorySystemComponent::SubItem(int InventorySlotIndex)
{
	GetPlayerState()->GetInventory()->InventoryItems[InventorySlotIndex].Reset();
	/*if (InventoryWidget)
		InventoryWidget->RemoveItem(InventorySlotIndex);*/

}

int UInventorySystemComponent::AutoEquipItems(int InvetorySlotIndex)
{
	int EquipSlotIndex = GetPlayerState()->GetInventory()->EquipItems[6].IsValid() && GetPlayerState()->GetInventory()->InventoryItems[InvetorySlotIndex].ItemType == EItemType::Tp_Ring ? 
		7 :
		(int)GetPlayerState()->GetInventory()->InventoryItems[InvetorySlotIndex].ItemType;
	
	FST_Item TempItem = GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex];
	GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex] = GetPlayerState()->GetInventory()->InventoryItems[InvetorySlotIndex];
	GetPlayerState()->GetInventory()->InventoryItems[InvetorySlotIndex].Reset();
	AddItem(TempItem);
	return EquipSlotIndex;
}

FInventory* UInventorySystemComponent::GetAllItems()
{
	return GetPlayerState()->GetInventory();
}

TArray<FST_Item> UInventorySystemComponent::GetInventoryItems()
{
	return GetPlayerState()->GetInventory()->InventoryItems;
}

TArray<FST_Item> UInventorySystemComponent::GetEquipItems()
{
	return GetPlayerState()->GetInventory()->EquipItems;
}

TArray<FStat> UInventorySystemComponent::GetEquipItemStat()
{
	TArray<FStat> stats;
	for (auto item : GetPlayerState()->GetInventory()->EquipItems)
	{
		stats.Add(item.Stat);
	}
	return stats;
}

FST_Item* UInventorySystemComponent::GetInventoryItem(int InventorySlotIndex)
{
	//FST_Item* Temp = &GetPlayerState()->GetInventory()->InventoryItems[0];

	if (GetPlayerState()->GetInventory()->InventoryItems.IsValidIndex(InventorySlotIndex))
		return (GetPlayerState()->GetInventory()->InventoryItems.GetData() + InventorySlotIndex);
	else
		return nullptr;
}

FST_Item* UInventorySystemComponent::GetEquipItem(int EquipSlotIndex)
{
	//FST_Item* Temp = &GetPlayerState()->GetInventory()->EquipItems[0];
	
	if (GetPlayerState()->GetInventory()->EquipItems.IsValidIndex(EquipSlotIndex))
		return (GetPlayerState()->GetInventory()->EquipItems.GetData() + EquipSlotIndex);
	else
		return nullptr;
}

void UInventorySystemComponent::UseItem(int InventorySlotIndex, int EquipSlotIndex, bool& isEquip)
{
	switch (GetPlayerState()->GetInventory()->InventoryItems[InventorySlotIndex].Category)
	{
	case EItemCategory::Cg_Equipment:
		EquipItem(InventorySlotIndex, EquipSlotIndex);
		isEquip = true;
		break;
	case EItemCategory::Cg_Consumable:
		isEquip = false;
		break;
	}
}

void UInventorySystemComponent::ClickUnEquipItem(int EquipSlotIndex)
{
	AddItem(GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex]);
	GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex].Reset();
}
void UInventorySystemComponent::DropUnEquipItem(int InventorySlotIndex, int EquipSlotIndex)
{
	FST_Item TempItem = GetPlayerState()->GetInventory()->InventoryItems[InventorySlotIndex];
	GetPlayerState()->GetInventory()->InventoryItems[InventorySlotIndex] = GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex];
	GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex] = TempItem;
	//InventoryWidget->SwapInvneoryAndEquip(InventorySlotIndex, EquipSlotIndex);
}
void UInventorySystemComponent::SwapInventoryItem(int ToItemSlotIndex, int FromItemSlotIndex)
{
	GetPlayerState()->GetInventory()->InventoryItems.Swap(FromItemSlotIndex, ToItemSlotIndex);
}
int UInventorySystemComponent::EquipItem(int InventorySlotIndex, int EquipSlotIndex)
{
	FST_Item TempItem = GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex];
	GetPlayerState()->GetInventory()->EquipItems[EquipSlotIndex] = GetPlayerState()->GetInventory()->InventoryItems[InventorySlotIndex];
	GetPlayerState()->GetInventory()->InventoryItems[InventorySlotIndex].Reset();
	AddItem(TempItem);

	return EquipSlotIndex;
}

void UInventorySystemComponent::EquipTwoHandItem(int InventorySlotIndex, int EquipSlotIndex)
{

}

void UInventorySystemComponent::SaveInventory()
{
}

void UInventorySystemComponent::LoadInventory(USG_PlayerData* SaveObject)
{
	GetPlayerState()->GetInventory()->EquipItems = SaveObject->GetData()->EquipItems;
	GetPlayerState()->GetInventory()->InventoryItems = SaveObject->GetData()->InventoryItems;
}
