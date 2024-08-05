// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Chat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UUW_Chat : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void AddChatMessage(const FString& Message);
	TSharedPtr<class SWidget> GetChatInputTextObejct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* SB_ChatList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* TB_SendMessage;

private:
	UFUNCTION()
	void OnChatMessageCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};