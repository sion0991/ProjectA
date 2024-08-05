// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "CommonState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UCommonState : public UStateBase
{
	GENERATED_BODY()
public:
	//virtual void UpdateState() override;
	//virtual void ExitState() override;

	virtual void InputMove(FVector2D MoveVector) override;
	virtual void InputJump() override;
};
