// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityInputConfig.generated.h"

//struct FGameplayTag;
//class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityInputMapping
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
		FGameplayTag InputTag;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGameplayAbility> Ability;
};

/**
 * 
 */
UCLASS()
class PROJECTA_API UAbilityInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//Returns the first Input Action associated with a given tag.
	const TSubclassOf<UGameplayAbility> FindAbilityForTag(const FGameplayTag& InputTag) const;
	const int FindAbilityIDForTag(const FGameplayTag& InputTag) const;

	//List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
		TArray<FAbilityInputMapping> AbilityInputMappings;

};
