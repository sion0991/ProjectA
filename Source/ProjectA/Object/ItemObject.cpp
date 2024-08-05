// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/Player/C_Player.h"
#include "Util/DataFunctionLibrary.h"
#include "Data/ST_Item.h"
#include "UI/UW_ItemText.h"


AItemObject::AItemObject()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetSphereRadius(256.0f);
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetRelativeRotation(FRotator(0, 0, 90.0f));
	SkeletalMesh->SetCollisionProfileName("NoCollision");
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(SkeletalMesh);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetVisibility(false);
	InteractType = 0;
	static ConstructorHelpers::FClassFinder<UUW_ItemText> FindUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/CUW_ItemText.CUW_ItemText_C'"));
	if (FindUI.Succeeded())
		Widget->SetWidgetClass(FindUI.Class);
}

void AItemObject::BeginPlay()
{	
	Super::BeginPlay();
	/*if (IsTest)
		SetRandomItemData(TestSpawnItemDatas);*/
}

void AItemObject::InteractEvent(AC_Player* Player)
{
	Player->AddItem(&ItemInfo);
	Destroy();
}

void AItemObject::SetWidgetVisibility(bool IsVisible)
{
	Widget->SetVisibility(IsVisible);
}
int AItemObject::GetInteractableType()
{
	return InteractType;
}
void AItemObject::SetItemData(FSpawnItemData ItemData)
{
	FName ItemName = ItemData.ItemName;

	/*FString Error;
	ItemInfo = *DataFunctionLibrary::GetItemTable()->FindRow<FST_Item>(ItemName, Error);
	if (Error.IsEmpty())
		SpawnItem(ItemInfo);*/

	UDataTable* ItemTable = DataFunctionLibrary::GetItemTable();
	if (ItemTable)
	{
		TArray<FST_Item*> Items;
		ItemTable->GetAllRows<FST_Item>(TEXT("GetAll"), Items);
		ItemInfo = *Items[FCString::Atoi(*ItemData.ItemName.ToString())];

		if (ItemInfo.IsValid())
			SpawnItem(ItemInfo);
	}
}

void AItemObject::SetRandomItemData(TArray<FSpawnItemData> ItemDatas)
{
	int RandomIndex = FMath::RandRange(0, ItemDatas.Num() - 1);

	SetItemData(ItemDatas[RandomIndex]);
}

void AItemObject::SpawnItem(const FST_Item& _ItemInfo)
{
	SkeletalMesh->SetSkinnedAssetAndUpdate(_ItemInfo.SkeletalMesh);
	Widget->InitWidget();
	UUW_ItemText* ItemWidget = Cast<UUW_ItemText>(Widget->GetUserWidgetObject());
	if (ItemWidget)
		ItemWidget->SetText(_ItemInfo.PickUpText);
}