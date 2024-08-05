// Fill out your copyright notice in the Description page of Project Settings.


#include "RidingState.h"
#include "Character/Player/C_Player.h"
#include "Gameplay/PC_Player.h"
#include "Character/C_RidingObject.h"

void URidingState::EnterState(AC_Player* Player)
{
	Super::EnterState(Player);

	Riding = Cast<AC_RidingObject>(Player->InteractableObject);
	if (Riding)
	{
		PlayerReference->AttachToActor(Riding, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PlayerReference->SetActorRelativeTransform(FTransform::Identity);
		APC_Player* PlayerController = Cast<APC_Player>(PlayerReference->GetController());
		PlayerController->SetViewTarget(Riding);
		timerCallback.BindLambda([&] {
			PlayerReference->SetActorRelativeTransform(FTransform::Identity);
			Cast<APC_Player>(PlayerReference->GetController())->SwitchPossess(Riding);
			});
		GetWorld()->GetTimerManager().SetTimer(InputHandle, timerCallback,
			PlayerReference->GetCurrentMontage()->GetPlayLength() / Riding->GetCurrentMontage()->RateScale - 0.03f, false);
	}
}

void URidingState::ExitState()
{
	APC_Player* PlayerController = Cast<APC_Player>(Riding->GetController());
	Riding->DisableInput(PlayerController);
	
	timerCallback.BindLambda([&] {
		Cast<APC_Player>(Riding->GetController())->SwitchPossess(PlayerReference);
		PlayerReference->SetActorRelativeTransform(FTransform(Riding->GetActorForwardVector().Rotation(), Riding->GetActorLocation() + Riding->GetActorRightVector() * -80, FVector::OneVector));
		//PlayerReference->GetPlayerController()->SetViewTarget(PlayerReference);
		PlayerReference->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		PlayerReference->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		});
	GetWorld()->GetTimerManager().SetTimer(InputHandle, timerCallback, 
		PlayerReference->GetCurrentMontage()->GetPlayLength() / PlayerReference->GetCurrentMontage()->RateScale - 0.06f, false);
}