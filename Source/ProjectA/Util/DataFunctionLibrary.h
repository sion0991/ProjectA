// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class APC_Player;
class USG_PlayerData;
class UCharacterDataAsset;
class USpawnDataAsset;
class UDataTable;

class PROJECTA_API DataFunctionLibrary
{
private:
	static void ItemUpdate(USG_PlayerData* Data, FString SlotName, int UserIndex);
public:
	static void LoadGame(FString SlotName, int UserIndex, USG_PlayerData*& Data);
	static void SaveGame(APC_Player* Player, FString SlotName, int UserIndex);
	static USG_PlayerData* GetSaveData(bool& HasData, FString SlotName = "Data", int UserIndex = 0);
	template <typename T>
	static TArray<T*> FindActors(UWorld* InWorld);
	static UCharacterDataAsset* GetCharacterData();
	static TSubclassOf<AActor> GetSpawnActor(int SpawnIndex);
	static UDataTable* GetItemTable();
	template<typename Enumeration>
	FORCEINLINE static FName GetEnumDisplayNameToName(const Enumeration Enumer)
	{
		int32 ConvertEnum = static_cast<uint8>(Enumer);
		return FName(StaticEnum<Enumeration>()->GetDisplayNameTextByValue(ConvertEnum).ToString());
	}
	template <typename T>
	FORCEINLINE static void SetArrayElement(TArray<T>& item_array, int32 index, T item)
	{
		if (item_array.Num() - 1 < index)
			item_array.SetNum(index);

		item_array.Insert(item, index);
	}
};
