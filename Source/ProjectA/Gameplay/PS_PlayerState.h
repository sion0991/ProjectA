// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Data/Inventory.h"
#include "PS_PlayerState.generated.h"

struct FST_Item;
UCLASS()
class PROJECTA_API APS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APS_PlayerState();
	class UInventorySystemComponent* GetInventorySystemComponent() const;
	FInventory* GetInventory();
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class URPGAttributeSet* GetAttributeSet() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState")
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|UI")
	void ShowAbilityConfirmCancelText(bool ShowText);
	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetHP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetMaxHP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetHPRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetMP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetMaxMP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetMPRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetAP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetMaxAP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetAPRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetDamage() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetDeffense() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetStrength() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetAgility() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetIntelligence() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetLuck() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		int32 GetEXP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		int32 GetEXPBounty() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPlayerState|Attributes")
		int32 GetGoldBounty() const;

	void LoadStat(TArray<FST_Item> EquipItems);
	void UseDurationItem(const FST_Item& Item);
	void UseConsumableItem(const FST_Item& Item);
protected:
	UPROPERTY(EditAnywhere)
	class UInventorySystemComponent* InventorySystemComponent;
	UPROPERTY(EditAnywhere, Replicated)
 	FInventory Inventory;

	UPROPERTY(EditAnywhere)
	class URPGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class URPGAttributeSet* AttributeSet;

	FGameplayTag DeadTag;

	
	FDelegateHandle InventoryChangedDelegateHandle;
	FDelegateHandle HPChangedDelegateHandle;
	FDelegateHandle MaxHPChangedDelegateHandle;
	FDelegateHandle HPRegenRateChangedDelegateHandle;
	FDelegateHandle MPChangedDelegateHandle;
	FDelegateHandle MaxMPChangedDelegateHandle;
	FDelegateHandle MPRegenRateChangedDelegateHandle;
	FDelegateHandle APChangedDelegateHandle;
	FDelegateHandle MaxAPChangedDelegateHandle;
	FDelegateHandle APRegenRateChangedDelegateHandle;
	FDelegateHandle XPChangedDelegateHandle;
	FDelegateHandle GoldChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;
	FDelegateHandle DamageChangedDelegateHandle;
	FDelegateHandle DeffenseChangedDelegateHandle;
	FDelegateHandle StrengthChangedDelegateHandle;
	FDelegateHandle AgilityChangedDelegatedHandle;
	FDelegateHandle IntelligenceChangedDelegatedHandle;
	FDelegateHandle LuckChangedDelegatedHandle;

	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HPChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHPChanged(const FOnAttributeChangeData& Data);
	virtual void HPRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void MPChanged(const FOnAttributeChangeData& Data);
	virtual void MaxMPChanged(const FOnAttributeChangeData& Data);
	virtual void MPRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void APChanged(const FOnAttributeChangeData& Data);
	virtual void MaxAPChanged(const FOnAttributeChangeData& Data);
	virtual void APRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void EXPChanged(const FOnAttributeChangeData& Data);
	virtual void GoldChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void DamageChanged(const FOnAttributeChangeData& Data);
	virtual void DeffenseChanged(const FOnAttributeChangeData& Data);
	virtual void StrengthChanaged(const FOnAttributeChangeData& Data);
	virtual void AgilityChanged(const FOnAttributeChangeData& Data);
	virtual void IntelligenceChanged(const FOnAttributeChangeData& Data);
	virtual void LuckChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
