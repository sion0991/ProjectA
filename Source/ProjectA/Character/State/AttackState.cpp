// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackState.h"
#include "Character/Player/C_Player.h"
void UAttackState::InputMove(FVector2D MoveVector)
{
	const FRotator YawRotation(0, PlayerReference->Controller->GetControlRotation().Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PlayerReference->AddMovementInput(ForwardDirection, MoveVector.Y);
	PlayerReference->AddMovementInput(RightDirection, MoveVector.X);
}

void UAttackState::InputJump()
{
	PlayerReference->Jump();
}
