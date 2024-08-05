// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

URPGGameplayAbility::URPGGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void URPGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}