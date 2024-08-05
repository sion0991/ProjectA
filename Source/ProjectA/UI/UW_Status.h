// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Status.generated.h"

class UTextBlock;


UCLASS()
class PROJECTA_API UUW_Status : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HP;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HPRegen;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MP;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MPRegen;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Damage;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Deffense;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Strength;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Agility;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Intelligense;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Luck;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MoveSpeed;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CharacterLevel;
};
