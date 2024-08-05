// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_EndAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UAN_EndAction : public UAnimNotify
{
	GENERATED_BODY()

public:
	bool isConvertViewDir;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
