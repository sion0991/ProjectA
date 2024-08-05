// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHUDWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTA_API URPGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetRespawnCountdown(float RespawnTimeRemaining);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_AP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_HPRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_MPRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_APRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_XP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TB_Gold;

	/**
	* Attribute setters
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHealthRegenRate(float HealthRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxStamina(float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCurrentStamina(float CurrentStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetStaminaPercentage(float StaminaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetStaminaRegenRate(float StaminaRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxMana(float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetCurrentMana(float CurrentMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetManaRegenRate(float ManaRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetExperience(int32 EXP);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetHeroLevel(int32 HeroLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetGold(int32 Gold);


};
