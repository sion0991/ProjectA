// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonState.h"
#include "Character/Player/C_Player.h"
#include "Gameplay/PC_Player.h"
void UCommonState::InputMove(FVector2D MoveVector)
{
	if (PlayerReference->GetCurrentMontage())
		return;

	const FRotator YawRotation(0, PlayerReference->GetPlayerController()->GetControlRotation().Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PlayerReference->AddMovementInput(ForwardDirection, MoveVector.Y);
	PlayerReference->AddMovementInput(RightDirection, MoveVector.X);
}

void UCommonState::InputJump()
{
	PlayerReference->Jump();
}
