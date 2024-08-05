// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "I_PlayerAnim.h"
#include "PlayerAnimInstance.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTA_API UPlayerAnimInstance : public UAnimInstance, public II_PlayerAnim
{
	GENERATED_BODY()

public:
	//virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D MoveValue = FVector2D::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsRide;
};
