// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SG_PlayerData.generated.h"

struct FST_Item;
struct FInventory;
class APC_Player;


USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		TArray<FST_Item> EquipItems;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		TArray<FST_Item> InventoryItems;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		int Gold = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		float HP = 50;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		float MP = 50;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		float AP = 50;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		int Level = 1;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
		int EXP = 0;
};

UCLASS()
class PROJECTA_API USG_PlayerData : public USaveGame
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	FSaveData SaveData;
public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Data)
	bool HasData = false;
	USG_PlayerData() {}
	void Save(APC_Player* Player, const FInventory& Items);
	void Update();
	FSaveData* GetData();
};
