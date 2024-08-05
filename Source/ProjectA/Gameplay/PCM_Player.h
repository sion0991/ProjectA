// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PCM_Player.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API APCM_Player : public APlayerCameraManager
{
	GENERATED_BODY()

private:
	class AC_Player* Player;

	FVector3d AimSocketOffset = FVector3d(0, 35, 40);
	FVector3d NormalSocketOffset = FVector3d(0, 0, 20);
	float AimTargetArmLength = 200;
	float NormalTargetArmLength = 300;
	float AimCamFOV = 60;
	float NormalCamFOV = 90;
	float InterpSpeed = 8;

	FVector3d SocketOffset;
	float ArmLength;
	float FOV;

	void ZoomInOut(FVector3d TargetSocketOffset, float TargetArmLength, float TargetFOV);
public:
	virtual void PostInitializeComponents() override;
	virtual void UpdateCamera(float DeltaTime) override;
};
