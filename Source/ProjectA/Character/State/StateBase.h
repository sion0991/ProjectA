// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateBase.generated.h"

class AC_Player;
struct FST_Item;
class USkeletalMeshComponent;
UCLASS(Blueprintable)
class PROJECTA_API UStateBase : public UObject
{
	GENERATED_BODY()
protected:
	AC_Player* PlayerReference;
public:
	UFUNCTION(BlueprintCallable)
	virtual void EnterState(AC_Player* Player);
	
	virtual void UpdateState() {}
	UFUNCTION(BlueprintCallable)
	virtual void ExitState() {}

	virtual void InputMove(FVector2D MoveVector);
	virtual void InputLook(FVector2D LookVector);
	virtual void InputJump();
	void InputChangeEquip(FST_Item* item, USkeletalMeshComponent* EquipComponent);
};