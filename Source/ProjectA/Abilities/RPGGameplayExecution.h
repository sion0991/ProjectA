// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RPGGameplayExecution.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API URPGGameplayExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	URPGGameplayExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
