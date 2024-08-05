// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_Stat.h"
#include "ST_Item.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Cg_Equipment	UMETA(DisplayName = "Equipment"),
	Cg_Consumable	UMETA(DisplayName = "Consumable"),
	Cg_Material		UMETA(DisplayName = "Material")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Tp_RightWeapon		UMETA(DisplayName = "RightWeapon"),
	Tp_LeftWeapon		UMETA(DisplayName = "LeftWeapon"),
	Tp_TwoHandWeapon	UMETA(DisplayName = "TwoHandWeapon"),
	Tp_HeadArmor		UMETA(DisplayName = "HeadArmor"),
	Tp_BodyArmor		UMETA(DisplayName = "BodyArmor"),
	Tp_HandArmor		UMETA(DisplayName = "HandArmor"),
	Tp_FootArmor		UMETA(DisplayName = "FootArmor"),
	Tp_Ring				UMETA(DisplayName = "Ring"),
	Tp_Consumable		UMETA(DisplayName = "Consumable"),
	Tp_Material			UMETA(DisplayName = "Material")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Tp_Sword		UMETA(DisplayName = "Sword"),
	Tp_Hammer		UMETA(DisplayName = "Hammer"),
	Tp_Spear		UMETA(DisplayName = "Spear"),
	Tp_Sickle		UMETA(DisplayName = "Sickle"),
	Tp_Arrow			UMETA(DisplayName = "Arrow"),
	Tp_Staff		UMETA(DisplayName = "Staff"),
	Tp_TwinDagger	UMETA(DisplayName = "TwinDagger"),
	Tp_Shield		UMETA(DisplayName = "Shield")
};

//USTRUCT(BlueprintType, meta = (HasNativeBreak = "ProjectA.BreakItemStruct.BreakST_Item", HasNativeMake = "ProjectA.BreakItemStruct.MakeST_Item"))
USTRUCT(BlueprintType)
struct PROJECTA_API FST_Item : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemCategory Category;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText PickUpText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ActionText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FStat Stat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform BTransform;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform ATransform;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName BSocket;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ASocket;

	FST_Item(){}
	void Reset()
	{
		ItemIndex = "";
		Category = EItemCategory::Cg_Equipment;
		ItemType = EItemType::Tp_RightWeapon;
		WeaponType = EWeaponType::Tp_Sword;
		SkeletalMesh = NULL;
		Icon = NULL;
		PickUpText = FText();
		ActionText = FText();
		Stat.Reset();
		Duration = 0;
		BTransform = FTransform::Identity;
		ATransform = FTransform::Identity;
		BSocket = "";
		ASocket = "";
	}

	FST_Item(const FST_Item& Ref)
	{
		ItemIndex = Ref.ItemIndex;
		Category = Ref.Category;
		ItemType = Ref.ItemType;
		WeaponType = Ref.WeaponType;
		SkeletalMesh = Ref.SkeletalMesh;
		Icon = Ref.Icon;
		PickUpText = Ref.PickUpText;
		ActionText = Ref.ActionText;
		Stat = Ref.Stat;
		Duration = Ref.Duration;
		BTransform = Ref.BTransform;
		ATransform = Ref.ATransform;
		BSocket = Ref.BSocket;
		ASocket = Ref.ASocket;
	}
	
	bool IsValid()
	{
		if (ItemIndex != "")
			return true;

		return false;
	}
	~FST_Item()
	{
		//this->StaticStruct()->DestroyStruct(this);
	}
	FST_Item& operator=(const FST_Item& Ref)
	{
		if (this != &Ref)
		{
			if (this)
				this->~FST_Item();

			new (this) FST_Item(Ref);
		}
		return *this;
	}
};

//UCLASS()
//class PROJECTA_API UBreakItemStruct : public UObject
//{
//	GENERATED_BODY()
//public:
//	UFUNCTION(BlueprintPure, Category = "ST_Item", meta = (NativeBreakFunc))
//		FORCEINLINE void BreakST_Item(const FST_Item& Item, FName& ItemIndex, EItemCategory& Category, EItemType& ItemType, EWeaponType& WeaponType, class USkeletalMesh*& SkeletalMesh, class UTexture2D*& Icon, FText& PickUpText, FText& ActionText, FStat& Stat, float& Duration, FTransform& BTransform, FTransform& ATransform, FName& BSocket, FName& ASocket)
//	{
//		ItemIndex = Item.ItemIndex;
//		Category = Item.Category;
//		ItemType = Item.ItemType;
//		WeaponType = Item.WeaponType;
//		SkeletalMesh = Item.SkeletalMesh;
//		Icon = Item.Icon;
//		PickUpText = Item.PickUpText;
//		ActionText = Item.ActionText;
//		Stat = Item.Stat;
//		Duration = Item.Duration;
//		BTransform = Item.BTransform;
//		ATransform = Item.ATransform;
//		BSocket = Item.BSocket;
//		ASocket = Item.ASocket;
//	}
//	UFUNCTION(BlueprintPure, Category = "ST_Item", meta = (NativeMakeFunc))
//		FORCEINLINE FST_Item MakeST_Item(FName ItemIndex, EItemCategory Category, EItemType ItemType, EWeaponType WeaponType, class USkeletalMesh* SkeletalMesh, class UTexture2D* Icon, FText PickUpText, FText ActionText, FStat Stat, float Duration, FTransform BTransform, FTransform ATransform, FName BSocket, FName ASocket)
//	{
//		FST_Item Item;
//
//		Item.ItemIndex = ItemIndex;
//		Item.Category = Category;
//		Item.ItemType = ItemType;
//		Item.WeaponType = WeaponType;
//		Item.SkeletalMesh = SkeletalMesh;
//		Item.Icon = Icon;
//		Item.PickUpText = PickUpText;
//		Item.ActionText = ActionText;
//		Item.Stat = Stat;
//		Item.Duration = Duration;
//		Item.BTransform = BTransform;
//		Item.ATransform = ATransform;
//		Item.BSocket = BSocket;
//		Item.ASocket = ASocket;
//
//		return Item;
//	}
//};