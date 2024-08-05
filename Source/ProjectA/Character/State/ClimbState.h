// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "ClimbState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UClimbState : public UStateBase
{
	GENERATED_BODY()
	
public:
	virtual void InputMove(FVector2D MoveVector) override;
	virtual void InputJump() override;
};
