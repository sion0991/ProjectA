// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_PlayerData.h"
#include "Engine/DataTable.h"
#include "Util/DataFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "Gameplay/PC_Player.h"
#include "Gameplay/PS_PlayerState.h"
#include "InventorySystemComponent.h"
#include "ST_Item.h"
#include "Data/Inventory.h"

void USG_PlayerData::Save(APC_Player* Player, const FInventory& Items)
{
	APS_PlayerState* PS = Cast<APS_PlayerState>(Player->PlayerState);
	SaveData.EquipItems = PS->GetInventory()->EquipItems;
	SaveData.InventoryItems = PS->GetInventory()->InventoryItems;
	SaveData.Gold = PS->GetGold();
	SaveData.HP = PS->GetHP();
	SaveData.MP = PS->GetMP();
	SaveData.AP = PS->GetAP();
	SaveData.Level = PS->GetCharacterLevel();
	SaveData.EXP = PS->GetEXP();
	HasData = true;
}

void USG_PlayerData::Update()
{
	UDataTable* DataTable = DataFunctionLibrary::GetItemTable();
	if (!DataTable)
		return;

	for (int i = 0; i < SaveData.EquipItems.Num(); i++)
	{
		if (SaveData.EquipItems[i].IsValid())
		{
			if (DataTable)
			{
				FString Error;
				SaveData.EquipItems[i] = *DataTable->FindRow<FST_Item>(SaveData.EquipItems[i].ItemIndex, Error);

				/*if (!Error.IsEmpty())
					UE_LOG(LogTemp, Warning, TEXT("%s"), *Error);*/
			}
		}
	}

	for (int i = 0; i < SaveData.InventoryItems.Num(); i++)
	{
		if (SaveData.InventoryItems[i].IsValid())
		{
			if (DataTable)
			{
				FString Error;
				SaveData.InventoryItems[i] = *DataTable->FindRow<FST_Item>(SaveData.InventoryItems[i].ItemIndex, Error);

				/*if (!Error.IsEmpty())
					UE_LOG(LogTemp, Warning, TEXT("%s"), *Error);*/
			}
		}
	}
}

FSaveData* USG_PlayerData::GetData()
{
	return &SaveData;
}
