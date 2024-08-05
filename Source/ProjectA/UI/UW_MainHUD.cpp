// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_MainHUD.h"
#include "Character/Player/C_Player.h"
#include "Gameplay/PC_Player.h"
#include "UW_Inventory.h"
#include "UW_InventorySlot.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UUW_MainHUD::NativeConstruct()
{
	StatVisible = ESlateVisibility::Hidden;
	PlayerController = Cast<APC_Player>(GetOwningPlayer());
}

//void UUW_MainHUD::DropAtInventorySlot(UUW_InventorySlot* From, UUW_InventorySlot* To)
//{
//	From->SlotType == "Inventory" ?                                                                                                                                          
//		PlayerController->GetInventorySystemComponent()->SwapInventoryItem(From->SlotIndex, To->SlotIndex) :
//		Cast<AC_Player>(GetOwningPlayerPawn())->TakeOffEquipment(To->SlotIndex, From->SlotIndex);
//}
//
//void UUW_MainHUD::SetItem(int InventorySlotIndex)
//{
//	InventorySlots[InventorySlotIndex]->PickUpImage = FindItemAtSlot(InventorySlotIndex)->Icon;
//}
//
//void UUW_MainHUD::RemoveItem(int InventorySlotIndex)
//{
//	InventorySlots[InventorySlotIndex]->PickUpImage = NULL;
//}
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
//void UUW_MainHUD::SwapInvneoryAndEquip(int InventorySlotIndex, int EquipSlotIndex)
//{
//	InventorySlots[InventorySlotIndex]->PickUpImage = FindItemAtSlot(InventorySlotIndex)->Icon;
//	EquipmentSlots[EquipSlotIndex]->PickUpImage = FindItemAtSlot(EquipSlotIndex, false)->Icon;
//}
//
//void UUW_MainHUD::LoadInventory()
//{
//	UW_Inventory->LoadInventory();
//}
//
//FST_Item* UUW_MainHUD::FindItemAtSlot(int SlotIndex, bool isInventory)
//{
//	return isInventory ? 
//		PlayerController->GetInventorySystemComponent()->GetInventoryItem(SlotIndex) : 
//		PlayerController->GetInventorySystemComponent()->GetEquipItem(SlotIndex);
//}

void UUW_MainHUD::SetMaxHP(float MaxHP)
{
}

void UUW_MainHUD::SetCurrentHP(float CurrentHP)
{
	TB_HP->SetText(FText::AsNumber(CurrentHP));
}

void UUW_MainHUD::SetHPPercentage(float HPPercentage)
{
	PB_HP->SetPercent(HPPercentage);
}

void UUW_MainHUD::SetHPRegenRate(float HPRegenRate)
{
	//TB_HPRegen->SetText(FText::)
	TB_HPRegen->SetText(UKismetTextLibrary::Conv_DoubleToText(HPRegenRate, ERoundingMode::ToPositiveInfinity, false, true, 1, 324, 0, 0));
}

void UUW_MainHUD::SetMaxMP(float MaxMP)
{

}

void UUW_MainHUD::SetCurrentMP(float CurrentMP)
{
	TB_MP->SetText(FText::AsNumber(CurrentMP));
}

void UUW_MainHUD::SetMPPercentage(float MPPercentage)
{
	PB_MP->SetPercent(MPPercentage);
}

void UUW_MainHUD::SetMPRegenRate(float MPRegenRate)
{
	TB_MPRegen->SetText(UKismetTextLibrary::Conv_DoubleToText(MPRegenRate, ERoundingMode::ToPositiveInfinity, false, true, 1, 324, 0, 0));
}

void UUW_MainHUD::SetMaxAP(float MaxAP)
{
}

void UUW_MainHUD::SetCurrentAP(float CurrentAP)
{
	TB_AP->SetText(FText::AsNumber(CurrentAP));
}

void UUW_MainHUD::SetAPPercentage(float APPercentage)
{
	PB_AP->SetPercent(APPercentage);
}

void UUW_MainHUD::SetAPRegenRate(float APRegenRate)
{
	TB_APRegen->SetText(UKismetTextLibrary::Conv_DoubleToText(APRegenRate, ERoundingMode::ToPositiveInfinity, false, true, 1, 324, 0, 0));
}

void UUW_MainHUD::SetExperience(int32 Experience)
{
	TB_MPRegen->SetText(UKismetTextLibrary::Conv_IntToText(Experience));
}

void UUW_MainHUD::SetCharacterLevel(int32 CharacterLevel)
{
	TB_CharacterLevel->SetText(UKismetTextLibrary::Conv_IntToText(CharacterLevel));
}

void UUW_MainHUD::SetGold(int32 Gold)
{
	TB_Gold->SetText(UKismetTextLibrary::Conv_IntToText(Gold));
}
