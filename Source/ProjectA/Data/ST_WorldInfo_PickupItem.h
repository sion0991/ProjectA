// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ST_InventoryContents.h"
#include "ST_WorldInfo_PickupItem.generated.h"

USTRUCT()
struct PROJECTA_API FST_WorldInfo_PickupItem
{
	GENERATED_BODY()

public:
	FName ItemRowName;
	TArray<FST_InventoryContents> ItemContents;
	FTransform WorldTransform;

	FST_WorldInfo_PickupItem() {}
	FST_WorldInfo_PickupItem(FName _ItemRowName, TArray<FST_InventoryContents> _ItemContents, FTransform _WorldTransform)
		: ItemRowName(_ItemRowName), ItemContents(_ItemContents), WorldTransform(_WorldTransform){}
};
