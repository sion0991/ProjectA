// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Data/InputConfig.h"
#include "GameplayTagContainer.h"
#include "RPGEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API URPGEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename FuncType>
	void BindActionByTag(const UInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
	{
		check(InputConfig);

		if (const UInputAction* IA = InputConfig->FindAbilityInputActionForTag(InputTag))
			BindAction(IA, TriggerEvent, Object, Func);
	}

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
	{
		check(InputConfig);

		for (const FTaggedInputAction& Action : InputConfig->AbilityInputActions)
		{
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				if (PressedFunc)
					BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());

				if (ReleasedFunc)
					BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
};

//template<class UserClass, typename FuncType>
//void URPGEnhancedInputComponent::BindActionByTag(const UInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
//{
//	check(InputConfig);
//
//	if (const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag))
//		BindAction(IA, TriggerEvent, Object, Func);
//}