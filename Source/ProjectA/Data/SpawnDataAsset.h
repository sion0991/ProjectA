// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTA_API USpawnDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> SpawnActor;

	//FORCEINLINE virtual FPrimaryAssetId GetPrimaryAssetId() const override
	//{
	//	return FPrimaryAssetId(GetClass()->GetFName(), GetOutermost()->GetFName());
	//}
};
