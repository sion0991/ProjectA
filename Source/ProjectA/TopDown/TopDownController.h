// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownCharacter.h"
#include "TopDownController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ATopDownController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, ATopDownCharacter* TargetCharacter);
	void ShowDamageNumber_Implementation(float DamageAmount, ATopDownCharacter* TargetCharacter);
	bool ShowDamageNumber_Validate(float DamageAmount, ATopDownCharacter* TargetCharacter);

	// Simple way to RPC to the client the countdown until they respawn from the GameMode. Will be latency amount of out sync with the Server.
	UFUNCTION(Client, Reliable, WithValidation)
	void SetRespawnCountdown(float RespawnTimeRemaining);
	void SetRespawnCountdown_Implementation(float RespawnTimeRemaining);
	bool SetRespawnCountdown_Validate(float RespawnTimeRemaining);
protected:
	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
