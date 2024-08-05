// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ST_InventoryContents.generated.h"

USTRUCT(BlueprintType)
struct FST_InventoryContents
{
	GENERATED_BODY()

public:
	FST_InventoryContents() {}
	FST_InventoryContents(FName _ItemRowName, int _ItemAmount)
		:ItemRowName(_ItemRowName), ItemAmount(_ItemAmount) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemAmount = 0;
};