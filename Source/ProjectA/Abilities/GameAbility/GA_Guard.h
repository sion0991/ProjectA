// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RPGGameplayAbility.h"
#include "GA_Guard.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UGA_Guard : public URPGGameplayAbility
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int MontageNum = -1;
public:
	UGA_Guard();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;


protected:
	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
