// Fill out your copyright notice in the Description page of Project Settings.


#include "PCM_Player.h"
#include "Character/Player/C_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

void APCM_Player::ZoomInOut(FVector3d TargetSocketOffset, float TargetArmLength, float TargetFOV)
{
	Player->GetCameraBoom()->SocketOffset = TargetSocketOffset;
	Player->GetCameraBoom()->TargetArmLength = TargetArmLength;
	Player->GetCamera()->FieldOfView = TargetFOV;
}

void APCM_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Player = Cast<AC_Player>(GetOwner());
}

void APCM_Player::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	/*if (Player->GetIsAim())
	{
		FOV = AimCamFOV;
		ArmLength = AimTargetArmLength;
		SocketOffset = AimSocketOffset;
	}
	else
	{
		FOV = NormalCamFOV;
		ArmLength = NormalTargetArmLength;
		SocketOffset = NormalSocketOffset;
	}
	FVector3d TargetSocketOffset = FMath::VInterpTo(Player->GetCameraBoom()->SocketOffset, SocketOffset, DeltaTime, InterpSpeed);
	float TargetArmLength = FMath::FInterpTo(Player->GetCameraBoom()->TargetArmLength, ArmLength, DeltaTime, InterpSpeed);
	float TargetFOV = FMath::FInterpTo(Player->GetCamera()->FieldOfView, FOV, DeltaTime, InterpSpeed);
	ZoomInOut(TargetSocketOffset, TargetArmLength, TargetFOV);*/
}
