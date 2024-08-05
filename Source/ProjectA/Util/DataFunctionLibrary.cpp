// Fill out your copyright notice in the Description page of Project Settings.


#include "DataFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/AssetManager.h"
#include "Gameplay/PC_Player.h"
#include "Data/InventorySystemComponent.h"
#include "Data/CharacterDataAsset.h"
#include "Data/SpawnDataAsset.h"
#include "Data/SG_PlayerData.h"
#include "Data/ST_Item.h"



void DataFunctionLibrary::ItemUpdate(USG_PlayerData* Data, FString SlotName, int UserIndex)
{
	Data->Update();
	UGameplayStatics::SaveGameToSlot(Data, SlotName, UserIndex);
}

void DataFunctionLibrary::LoadGame(FString SlotName, int UserIndex, USG_PlayerData*& Data)
{
	bool HasData = false;
	Data = GetSaveData(HasData, SlotName, UserIndex);
	if (HasData)
		ItemUpdate(Data, SlotName, UserIndex);
}

void DataFunctionLibrary::SaveGame(APC_Player* Player, FString SlotName, int UserIndex)
{
	bool HasData = false;
	USG_PlayerData* Data = GetSaveData(HasData, SlotName, UserIndex);
	Data->Save(Player, *Player->GetInventorySystemComponent()->GetAllItems());
	UGameplayStatics::SaveGameToSlot(Data, SlotName, UserIndex);
}

USG_PlayerData* DataFunctionLibrary::GetSaveData(bool& HasData, FString SlotName, int UserIndex)
{
	HasData = UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
	USG_PlayerData* SaveGame = Cast<USG_PlayerData>(UGameplayStatics::CreateSaveGameObject(USG_PlayerData::StaticClass()));
	if (HasData)
		SaveGame = Cast<USG_PlayerData>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	return SaveGame;
}

template <typename T>
TArray<T*> DataFunctionLibrary::FindActors(UWorld* InWorld)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), OutActors);
	InWorld->get
	TArray<T*> ResultActors;
	for (AActor* Actor : OutActors)
	{
		ResultActors.Add(Cast<T>(Actor));
	}
	return ResultActors;
}

UCharacterDataAsset* DataFunctionLibrary::GetCharacterData()
{
	static ConstructorHelpers::FObjectFinder<UCharacterDataAsset> CData(TEXT("CharacterDataAsset'/Game/Blueprints/Info/CharacerData.CharacerData'"));
	if (CData.Succeeded())
	{
		return CData.Object;
	}
	/*UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UCharacterDataAsset* Data = Cast<UCharacterDataAsset>(Manager->GetPrimaryAssetObject(FPrimaryAssetId("CharacterData")));
		return Data;
	}*/
	return nullptr;
}

TSubclassOf<AActor> DataFunctionLibrary::GetSpawnActor(int SpawnIndex)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	USpawnDataAsset* SpawnData;
	if (Manager)
	{
		SpawnData = Cast<USpawnDataAsset>(Manager->GetPrimaryAssetObject(FPrimaryAssetId("SpawnData:SpawnData")));
		if (SpawnData && !SpawnData->SpawnActor.IsEmpty())
			return SpawnData->SpawnActor[SpawnIndex];
	}
	UE_LOG(LogTemp, Warning, TEXT("DataFunctionLibrary - NotFound SpawnDataAsset"));
	return nullptr;
}
UDataTable* DataFunctionLibrary::GetItemTable()
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
	UDataTable* DataTable;
	if (Manager)
	{
		DataTable = Cast<UDataTable>(Manager->GetPrimaryAssetObject(FPrimaryAssetId("DataTable:ItemTable")));
		if (DataTable)
			return DataTable;
	}
	UE_LOG(LogTemp, Warning, TEXT("DataFunctionLibrary - NotFound DataTable"));
	return nullptr;
}

//template<typename Enumeration>
//FName DataFunctionLibrary::GetEnumDisplayNameToName(const Enumeration Enumer)
//{
//	int32 ConvertEnum = static_cast<uint8>(Enumer);
//	return FName(StaticEnum<Enumeration>()->GetDisplayNameTextByValue(ConvertEnum).ToString());
//}