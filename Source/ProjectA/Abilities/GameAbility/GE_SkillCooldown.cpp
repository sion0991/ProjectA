// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_SkillCooldown.h"
#include "GameplayTagContainer.h"

UGE_SkillCooldown::UGE_SkillCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	//DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(2.0f));

	/*InheritableOwnedTagsContainer.CombinedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Sprint")));
	InheritableOwnedTagsContainer.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Sprint")));*/
}

void UGE_SkillCooldown::PostLoad()
{
	Super::PostLoad();

	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(Cooldown));
	InheritableOwnedTagsContainer.CombinedTags.AddTag(CooldownTag);
	InheritableOwnedTagsContainer.Added.AddTag(CooldownTag);
}
