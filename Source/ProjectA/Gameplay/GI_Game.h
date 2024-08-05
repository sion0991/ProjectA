// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/ST_WorldInfo_PickupItem.h"
#include "GI_Game.generated.h"

class USG_GameWorld;

UCLASS()
class PROJECTA_API UGI_Game : public UGameInstance
{
	GENERATED_BODY()
	
public:
	FString GameWorldSaveSlot;
public:
	UGI_Game();
	~UGI_Game();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

public:
	USG_GameWorld* InitializeGameWorldSave(FString SaveSlotName);
	void UpdateSavePickupActors(TArray<FST_WorldInfo_PickupItem> AllPickupActorsInfo);

};