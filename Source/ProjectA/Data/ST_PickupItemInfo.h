// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_PickupItemInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTA_API FST_PickupItemInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMesh* Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MeshScale = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector MeshOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Icon = nullptr;
};
