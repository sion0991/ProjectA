// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Game.h"
#include "GI_Game.h"
#include "Data/SG_GameWorld.h"
#include "Object/ItemObject.h"
#include "Util/DataFunctionLibrary.h"

void AGM_Game::BeginPlay()
{
	GameInstance = Cast<UGI_Game>(UGameplayStatics::GetGameInstance(GetWorld()));
	USG_GameWorld* SaveGame = GameInstance->InitializeGameWorldSave(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	if (SaveGame)
	{
		SetUpPickUpActors(SaveGame);
	}
}

void AGM_Game::SetUpPickUpActors(USG_GameWorld* SG_GameWorld)
{
	/*if (SG_GameWorld->isFirstTimeLoadingMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Saved PickUp Data. Saving World Info to SaveGame"));
		
		AllSavedPickupActors = DataFunctionLibrary::FindActors<AItemObject>(GetWorld());

		for (int i = 0; i < AllSavedPickupActors.Num(); i++)
		{
			FST_WorldInfo_PickupItem Item = FST_WorldInfo_PickupItem(AllSavedPickupActors[i]->ItemRowName,
																	 AllSavedPickupActors[i]->ItemContents,
																	 AllSavedPickupActors[i]->GetActorTransform());

			DataFunctionLibrary::SetArrayElement<FST_WorldInfo_PickupItem>(AllSavedPickupActorsInfo,i, Item);
		}
		SG_GameWorld->isFirstTimeLoadingMap = false;
		GameInstance->UpdateSavePickupActors(AllSavedPickupActorsInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved Pickup Data found. Loading World Contents"));
		TArray<AActor*> PickUpActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemObject::StaticClass(), PickUpActors);
		for (AActor* p : PickUpActors)
		{
			p->Destroy();
		}

		for (FST_WorldInfo_PickupItem p : SG_GameWorld->AllSavedPickupActorsInfo)
		{
			AItemObject* Item = GetWorld()->SpawnActor<AItemObject>(AItemObject::StaticClass(), p.WorldTransform);
			Item->ItemRowName = p.ItemRowName;
			Item->ItemContents = p.ItemContents;

			DataFunctionLibrary::SetArrayElement<FST_WorldInfo_PickupItem>(AllSavedPickupActorsInfo, AllSavedPickupActors.Add(Item),p);
			
		}
	}*/
}

void AGM_Game::AddSavedPickupActor(AItemObject* PickupItem)
{
	//int AddIndex = AllSavedPickupActors.AddUnique(PickupItem);
	//if (AddIndex >= 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Pickup Added to SaveGame"));

	//	PickupItem->GetItemCategory() == EItemCategory::Cg_Equipment ?
	//		AllSavedPickupActorsInfo.Add(PickupItem->GetInfo()) :
	//		0;
	//		/*DataFunctionLibrary::SetArrayElement<FST_Item>(AllSavedPickupActorsInfo, AddIndex,
	//			FST_Item(PickupItem->ItemRowName, PickupItem->ItemContents, PickupItem->GetActorTransform()));*/

	//	GameInstance->UpdateSavePickupActors(AllSavedPickupActorsInfo);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Adding Pickup to SaveGame Failed"));
	//}
}

void AGM_Game::RemoveSavedPickupActor(AItemObject* PickupItem)
{
	/*int IndextoRemove = AllSavedPickupActors.Find(PickupItem);
	if (IndextoRemove >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup Remove From SaveGame"));
		AllSavedPickupActorsInfo.RemoveAt(IndextoRemove);
		AllSavedPickupActors.RemoveAt(IndextoRemove);
		GameInstance->UpdateSavePickupActors(AllSavedPickupActorsInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup Remove From SaveGame Failed"));
	}*/
}