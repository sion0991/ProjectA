// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAssetManager.h"
//#include "RPGGameplayTags.h"

URPGAssetManager& URPGAssetManager::Get()
{
	check(GEngine);

	URPGAssetManager* RPGAssetManager = Cast<URPGAssetManager>(GEngine->AssetManager);
	if(RPGAssetManager)
		return *RPGAssetManager;
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Assetmanager in DefaultEngine.ini, Must set this to RPGAssetManager"));
		return *NewObject<URPGAssetManager>();
	}
}

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//Native Tags �ε�
	//FRPGGameplayTags::InitailizeNativeTags();
}
