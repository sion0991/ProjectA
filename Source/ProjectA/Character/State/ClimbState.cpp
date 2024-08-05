// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbState.h"
#include "Character/Player/C_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

void UClimbState::InputMove(FVector2D MoveVector)
{
	const FRotator YawRotation = FRotator(0,PlayerReference->Controller->GetControlRotation().Yaw,0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PlayerReference->AddMovementInput(PlayerReference->GetActorUpVector(), MoveVector.Y);
	PlayerReference->AddMovementInput(PlayerReference->GetActorRightVector(), MoveVector.X);
}

void UClimbState::InputJump()
{
	FVector velocity = PlayerReference->GetActorForwardVector() * -100.0f;
	PlayerReference->LaunchCharacter(velocity,true,false);
	PlayerReference->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	PlayerReference->GetCharacterMovement()->bOrientRotationToMovement = true;
	PlayerReference->ChangeState(EStateType::Common);
}