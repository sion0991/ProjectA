// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

class USkeletalMesh;
class UAnimInstance;
class UAnimMontage;
class UInventorySystemComponent;
class UStateBase;
UCLASS(Blueprintable, BlueprintType)
class PROJECTA_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UStateBase>> State;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySystemComponent> Inventory;
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AnimMontages;
};
