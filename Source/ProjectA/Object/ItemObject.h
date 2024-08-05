// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/I_Interactable.h"
#include "GameFramework/Actor.h"
#include "Data/ST_Item.h"
#include "ItemObject.generated.h"

class UPrimitiveComponent;
class USphereComponent;
class USkeletalMeshComponent;
class UWidgetComponent;
class AC_Player;

USTRUCT(BlueprintType)
struct FSpawnItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Amount = 1;
};


UCLASS()
class PROJECTA_API AItemObject : public AActor, public II_Interactable
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* Trigger;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FST_Item ItemInfo;
	int ItemIndex = 0;

	int InteractType;
	int Amount;
	
public:
	AItemObject();
protected:
	virtual void BeginPlay() override;
public:
	virtual void InteractEvent(AC_Player* Player) override;
	virtual void SetWidgetVisibility(bool IsVisible) override;
	virtual int GetInteractableType() override;
	void SetItemData(FSpawnItemData ItemData);
	void SetRandomItemData(TArray<FSpawnItemData> ItemDatas);
	void SpawnItem(const FST_Item& _ItemInfo);
	EItemCategory GetItemCategory() { return ItemInfo.Category; }
	FST_Item GetInfo() { return ItemInfo; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSpawnItemData> TestSpawnItemDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTest = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRandom = true;
};
