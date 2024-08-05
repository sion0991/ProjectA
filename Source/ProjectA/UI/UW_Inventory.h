// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Inventory.generated.h"

class UUW_InventorySlot;
class UCanvasPanel;
class UUniformGridPanel;
struct FST_Item;

UCLASS()
class PROJECTA_API UUW_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<UUW_InventorySlot*> EquipmentSlots;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<UUW_InventorySlot*> InventorySlots;

	//UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	//	TArray<TSubclassOf<UUW_InventorySlot>> EquipmentSlots;
	//UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	//	TArray<TSubclassOf<UUW_InventorySlot>> InventorySlots;
	class APC_Player* PlayerController;
	class UInventorySystemComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCanvasPanel* CP_Equipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UUniformGridPanel* GP_InventoryGrid;

	bool DragOut = false;
	TSubclassOf<UUW_InventorySlot> SlotWidget;
	bool DoOnce = false;
public:
	UUW_Inventory(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void Init(APC_Player* player);
	void LoadInventory();
private:
	FST_Item* FindItemAtSlot(int SlotIndex, bool isInventory = true);

	void UpdateInventory();
public:
	UFUNCTION()
	void DropAtInventorySlot(UUW_InventorySlot* From, UUW_InventorySlot* To);
	UFUNCTION()
	void DropItem(UUW_InventorySlot* DropItemSlot);
	UFUNCTION()
	void DropAtEquipSlot(UUW_InventorySlot* From, UUW_InventorySlot* To);
	void SetItem(int InventorySlotIndex);
	void RemoveItem(int InventorySlotIndex);
	void SwapInvneoryAndEquip(int InventorySlotIndex, int EquipSlotIndex);
};
