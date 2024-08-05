// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Interact.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PROJECTA_API UUW_Interact : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ESlateVisibility> InteractVisible;
};
