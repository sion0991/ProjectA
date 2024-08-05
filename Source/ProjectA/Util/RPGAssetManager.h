// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RPGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API URPGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	

public:
	static URPGAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;
};
