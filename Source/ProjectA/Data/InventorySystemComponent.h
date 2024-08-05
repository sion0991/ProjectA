// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"

struct FST_Item;
struct FInventory;
class USG_PlayerData;
class APS_PlayerState;

UCLASS(Blueprintable)
class PROJECTA_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	APS_PlayerState* GetPlayerState();
public:
	UFUNCTION(BlueprintCallable)
	void AddItem(const FST_Item& Item);
	UFUNCTION(BlueprintCallable)
	void SubItem(int InvetorySlotIndex);
	int AutoEquipItems(int InvetorySlotIndex);
	FInventory* GetAllItems();
	TArray<FST_Item> GetInventoryItems();
	TArray<FST_Item> GetEquipItems();
	TArray<struct FStat> GetEquipItemStat();
	FST_Item* GetInventoryItem(int InvetorySlotIndex);
	FST_Item* GetEquipItem(int EquipSlotIndex);
	void UseItem(int InventorySlotIndex, int EquipSlotIndex, bool& isEquip);
	void ClickUnEquipItem(int EquipSlotIndex);
	void DropUnEquipItem(int InventorySlotIndex, int EquipSlotIndex);
	void SwapInventoryItem(int ToItemSlotIndex, int FromItemSlotIndex);
	int EquipItem(int InventorySlotIndex, int EquipSlotIndex);
	void EquipTwoHandItem(int InventorySlotIndex, int EquipSlotIndex);
public:
	void SaveInventory();
	void LoadInventory(USG_PlayerData* SaveObject);
};
