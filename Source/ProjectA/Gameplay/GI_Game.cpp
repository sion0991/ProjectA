// Fill out your copyright notice in the Description page of Project Settings.

#include "GI_Game.h"
#include "Data/SG_GameWorld.h"
#include "Kismet/GameplayStatics.h"

UGI_Game::UGI_Game()
{
}

UGI_Game::~UGI_Game()
{
}

void UGI_Game::Init()
{
	Super::Init();
}

void UGI_Game::Shutdown()
{
	Super::Shutdown();
}

void UGI_Game::StartGameInstance()
{
	Super::StartGameInstance();
}

USG_GameWorld* UGI_Game::InitializeGameWorldSave(FString SaveSlotName)
{
	GameWorldSaveSlot = SaveSlotName;
	USG_GameWorld* SG_GameWorld;
	if (UGameplayStatics::DoesSaveGameExist(GameWorldSaveSlot, 0))
	{
		SG_GameWorld = Cast<USG_GameWorld>(UGameplayStatics::LoadGameFromSlot(GameWorldSaveSlot, 0));
	}
	else
	{
		SG_GameWorld = Cast<USG_GameWorld>(UGameplayStatics::CreateSaveGameObject(USG_GameWorld::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SG_GameWorld, GameWorldSaveSlot, 0);
	}
	return SG_GameWorld;
}

void UGI_Game::UpdateSavePickupActors(TArray<FST_WorldInfo_PickupItem> AllPickupActorsInfo)
{
	USG_GameWorld* SG_GameWorld = Cast<USG_GameWorld>(UGameplayStatics::LoadGameFromSlot(GameWorldSaveSlot, 0)); 
	SG_GameWorld->AllSavedPickupActorsInfo = AllPickupActorsInfo;
	/*SG_GameWorld->All_pickupActorsInfo.Reset(0);
	SG_GameWorld->All_pickupActorsInfo.Append(AllPickupActorsInfo);*/
	UGameplayStatics::SaveGameToSlot(SG_GameWorld, GameWorldSaveSlot, 0);
}