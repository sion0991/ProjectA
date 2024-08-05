// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/ST_Item.h"
#include "GM_Game.generated.h"

class UGI_Game;
class AItemObject;
class USG_GameWorld;

UCLASS()
class PROJECTA_API AGM_Game : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	UGI_Game* GameInstance;
	TArray<AItemObject*> AllSavedPickupActors;
	TArray<FST_Item> AllSavedPickupActorsInfo;
public:
	virtual void BeginPlay() override;
	void SetUpPickUpActors(USG_GameWorld* SG_GameWorld);
	void AddSavedPickupActor(AItemObject* PickupItem);
	void RemoveSavedPickupActor(AItemObject* PickupItem);
};
