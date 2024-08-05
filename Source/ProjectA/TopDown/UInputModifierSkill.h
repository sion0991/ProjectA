// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "UInputModifierSkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UUInputModifierSkill : public UInputModifier
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Settings)
	FName Hotkey;
protected:
	int a;
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
