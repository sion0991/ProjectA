// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "PC_Player.h"
#include "UI/UW_MainHUD.h"
#include "UI/UW_Chat.h"
#include "UI/UW_Inventory.h"
#include "UI/UW_Interact.h"
#include "Data/CharacterDataAsset.h"
#include "Blueprint/UserWidget.h"
#include "Data/ST_Item.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!DoOnce)
	{
		MainWidget = CheckUIObject<UUW_MainHUD>(MainWidget, Widgets[0]);
		MainWidget->AddToViewport(3);
		InventoryWidget = CheckUIObject<UUW_Inventory>(InventoryWidget, Widgets[1]);
		InteractWidget = CheckUIObject<UUW_Interact>(InteractWidget, Widgets[2]);
		InteractWidget->AddToViewport(3);
		DoOnce = true;
	}
}

void APlayerHUD::AddChatMessage(const FString& Message)
{
	if (!CheckUIObject(MainWidget, Widgets[0]))
		return;

	MainWidget->TB_Chat->AddChatMessage(Message);
		
}

TSharedPtr<class SWidget> APlayerHUD::GetChatInputTextObejct()
{
	return MainWidget->TB_Chat->GetChatInputTextObejct();
}

TSharedPtr<class SWidget> APlayerHUD::GetInventoryObject()
{
	return InventoryWidget->GetCachedWidget();
}

//bool APlayerHUD::CheckUIObject(const TSubclassOf<UUserWidget> Widget)
//{
//	if(MainWidget)
//		return true;
//	
//	return CreateUIObject();
//}
//
//bool APlayerHUD::CreateUIObject(const TSubclassOf<UUserWidget> Widget)
//{
//	AC_Player* Player = Cast<AC_Player>(GetOwningPawn());
//	if (!Player)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("APlayerHUD - Controller is Not"));
//		return false;
//	}
//
//	TSubclassOf<UUserWidget> MainWidget = Player->GetCharacterData()->UserWidgets[0];
//	if (MainWidget)
//	{
//		MainWidget = CreateWidget<UUW_MainHUD>(GetOwningPlayerController(), MainWidget);
//		if (MainWidget)
//			MainWidget->AddToViewport();
//	}
//	else
//		UE_LOG(LogTemp, Warning, TEXT("APlayerHUD - UserWidgets is not"));
//	return false;
//}
template<class T>
T* APlayerHUD::CheckUIObject(T* WidgetClass, const TSubclassOf<UUserWidget> Widget)
{
	if (WidgetClass)
		return WidgetClass;
	return CreateUIObject<T>(WidgetClass, Widget);
}

template<class T>
T* APlayerHUD::CreateUIObject(T* WidgetClass, const TSubclassOf<UUserWidget> Widget)
{
	if (Widget)
	{
		WidgetClass = CreateWidget<T>(GetOwningPlayerController(), Widget);
		if (WidgetClass)
		{
			return WidgetClass;
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("APlayerHUD - UserWidgets is not"));
	return nullptr;
}

void APlayerHUD::OnOffInventory()
{
	if (bOnInventory)
	{
		InventoryWidget->RemoveFromParent();
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
		GetOwningPlayerController()->SetShowMouseCursor(false);
		bOnInventory = false;
	}
	else
	{
		InventoryWidget->AddToViewport();
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetOwningPlayerController());
		GetOwningPlayerController()->SetShowMouseCursor(true);
		bOnInventory = true;
	}
		
}

void APlayerHUD::OnOffInteract(int Type, bool OnOff)
{
	InteractWidget->InteractVisible[Type] = OnOff ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
}
