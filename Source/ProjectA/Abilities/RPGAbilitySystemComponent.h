// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, URPGAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);
/**
 * 
 */
UCLASS()
class PROJECTA_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	bool bAllowAllInput = false;

	FReceivedDamageDelegate ReceivedDamage;

	// Called from GDDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(URPGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
	virtual void BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo) override;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	/** Called on local player always. Called on server only if bReplicateInputDirectly is set on the GameplayAbility. */
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

};