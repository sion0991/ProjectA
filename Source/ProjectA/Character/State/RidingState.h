// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/State/StateBase.h"
#include "RidingState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API URidingState : public UStateBase
{
	GENERATED_BODY()
	
private:
	class AC_RidingObject* Riding;
	FTimerHandle InputHandle;
	FTimerDelegate timerCallback;
public:
	virtual void EnterState(AC_Player* Player) override;
	virtual void ExitState() override;
};
