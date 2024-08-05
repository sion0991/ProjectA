// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputConfig.generated.h"

class UInputAction;
//struct FGameplayTag;
/**
 * 
 */


USTRUCT(BlueprintType)
struct FTaggedInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS(BlueprintType)
class PROJECTA_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//지정된 태그와 관련된 첫 번째 입력 작업을 반환
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

	//소유자가 사용하는 입력 작업 목록. 입력 작업은 게임 플레이 태그에 매핑되며 수동으로 바인딩해야함.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<UInputAction*> OtherInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FTaggedInputAction> AbilityInputActions;
};
