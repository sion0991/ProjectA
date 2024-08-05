// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ItemText.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UUW_ItemText : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UTextBlock* TB_PickUp;

public:
	UFUNCTION(BlueprintCallable)
	void SetText(FText Text);
};
