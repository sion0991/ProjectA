// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MainHUD.generated.h"

class UCanvasPanel;
class UUniformGridPanel;
class UProgressBar;
class UTextBlock;
class UUW_Chat;
class UUW_Inventory;
struct FST_Item;

UCLASS()
class PROJECTA_API UUW_MainHUD : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class APC_Player* PlayerController;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* PB_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* PB_MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* PB_AP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlateVisibility StatVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_HPRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_MPRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_AP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_APRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_EXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TB_CharacterLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUW_Chat* TB_Chat;

	virtual void NativeConstruct() override;
	/*void DropAtInventorySlot(UUW_InventorySlot* From, UUW_InventorySlot* To);
	void SetItem(int UW_InventorySlotIndex);
	void RemoveItem(int UW_InventorySlotIndex);
	void SwapInvneoryAndEquip(int UW_InventorySlotIndex, int EquipSlotIndex);
	void LoadInventory();
	FST_Item* FindItemAtSlot(int SlotIndex, bool isInventory = true);*/

	void SetMaxHP(float MaxHP); //������
	void SetCurrentHP(float CurrentHP);
	void SetHPPercentage(float HPPercentage);
	void SetHPRegenRate(float HPRegenRate);

	void SetMaxMP(float MaxMP);//������
	void SetCurrentMP(float CurrentMP);
	void SetMPPercentage(float MPPercentage);
	void SetMPRegenRate(float MPRegenRate);

	void SetMaxAP(float MaxAP);//������
	void SetCurrentAP(float CurrentAP);
	void SetAPPercentage(float APPercentage);
	void SetAPRegenRate(float APRegenRate);

	void SetExperience(int32 Experience);
	void SetCharacterLevel(int32 CharacterLevel);
	void SetGold(int32 Gold);
};
