// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UUW_Inventory;
class UUserWidget;
struct FST_Item;

UCLASS()
class PROJECTA_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UUserWidget>> Widgets;
	
private:
	template<class T>
	T* CheckUIObject(T* WidgetClass, const TSubclassOf<UUserWidget> Widget);
	template<class T>
	T* CreateUIObject(T* WidgetClass, const TSubclassOf<UUserWidget> Widget);

	bool bOnInventory = false;
	bool DoOnce = false;
public:
	virtual void DrawHUD() override;

	class UUW_MainHUD* MainWidget;
	class UUW_Inventory* InventoryWidget;
	class UUW_Interact* InteractWidget;

	void AddChatMessage(const FString& Message);
	TSharedPtr<class SWidget> GetChatInputTextObejct();
	TSharedPtr<class SWidget> GetInventoryObject();
	void OnOffInventory();
	void OnOffInteract(int Type, bool OnOff);
};