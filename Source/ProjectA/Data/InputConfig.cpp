// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConfig.h"
#include "EnhancedInputSubsystems.h"
//#include "GameplayTagContainer.h"

const UInputAction* UInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTaggedInputAction& TaggedInputAction : AbilityInputActions)
	{
		if (TaggedInputAction.InputAction && TaggedInputAction.InputTag == InputTag)
			return TaggedInputAction.InputAction;
	}
	return nullptr;
}
