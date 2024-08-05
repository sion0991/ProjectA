// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_EndAction.h"
#include "Character/Player/C_Player.h"

void UAN_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AC_Player* Player = Cast<AC_Player>(MeshComp->GetOwner());
	isConvertViewDir ?
		Player->bUseControllerRotationYaw = true :
		0;
	//Player->isCanAction = true;
}
