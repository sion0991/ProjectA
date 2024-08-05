// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_PlayerAnim.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UI_PlayerAnim : public UInterface
{
	GENERATED_BODY()
};
class PROJECTA_API II_PlayerAnim
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetInputMove(FVector2D Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsMove(bool bMove);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsRide(bool bRide);
	/*UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Equip();*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackCommon();
};
