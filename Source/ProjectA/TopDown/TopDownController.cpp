// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownController.h"
#include "Gameplay/PS_PlayerState.h"
#include "AbilitySystemComponent.h"

void ATopDownController::ShowDamageNumber_Implementation(float DamageAmount, ATopDownCharacter* TargetCharacter)
{
}

bool ATopDownController::ShowDamageNumber_Validate(float DamageAmount, ATopDownCharacter* TargetCharacter)
{
	return false;
}

void ATopDownController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
}

bool ATopDownController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return false;
}

void ATopDownController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APS_PlayerState* PS = GetPlayerState<APS_PlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void ATopDownController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//CreateHUD();
}
