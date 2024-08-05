// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ST_InventoryContents.h"
#include "SG_Inventory.generated.h"

//struct FST_InventoryContents;

UCLASS()
class PROJECTA_API USG_Inventory : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	TArray<FST_InventoryContents> Inventory;
};
