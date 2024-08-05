// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_SkillCooldown.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UGE_SkillCooldown : public UGameplayEffect
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	struct FGameplayTag CooldownTag;
public:
	UGE_SkillCooldown();
	virtual void PostLoad() override;
};
