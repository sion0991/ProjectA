// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_Character.generated.h"

struct FGameplayTag;
struct FInputActionValue;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Character : public UInterface
{
	GENERATED_BODY()
};

class PROJECTA_API II_Character
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Move(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Look(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InputAbilitySystemInputTagPressed(FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InputAbilitySystemInputTagReleased(FGameplayTag InputTag);
};
