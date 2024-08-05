// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ST_WorldInfo_PickupItem.h"
#include "SG_GameWorld.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API USG_GameWorld : public USaveGame
{
	GENERATED_BODY()
	
public:
	TArray<FST_WorldInfo_PickupItem> AllSavedPickupActorsInfo;
	bool isFirstTimeLoadingMap = true;
};
