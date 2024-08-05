// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/C_Player.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{	
	FVector2D MoveXY = FVector2D::ZeroVector;

	ACharacter* PawnOwner = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (PawnOwner)
	{
		AC_Player* Character = Cast<AC_Player>(TryGetPawnOwner());
		float Dir = PawnOwner->GetCharacterMovement()->bOrientRotationToMovement ?
			UKismetAnimationLibrary::CalculateDirection(PawnOwner->GetVelocity(), PawnOwner->GetActorRotation()):
			UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(PawnOwner->GetLastMovementInputVector()), PawnOwner->GetControlRotation()).Yaw;
		MoveValue = FVector2D(Dir, PawnOwner->GetVelocity().Length());

		IsInAir = PawnOwner->GetMovementComponent()->IsFalling();
	}
}
