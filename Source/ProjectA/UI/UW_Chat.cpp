// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Chat.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Gameplay/PC_Player.h"

void UUW_Chat::NativeConstruct()
{
	Super::NativeConstruct();

	TB_SendMessage->OnTextCommitted.AddDynamic(this, &UUW_Chat::OnChatMessageCommitted);
}

void UUW_Chat::AddChatMessage(const FString& Message)
{
	UTextBlock* Chat = NewObject<UTextBlock>(SB_ChatList);
	Chat->SetText(FText::FromString(Message));
	
	SB_ChatList->AddChild(Chat);
	SB_ChatList->ScrollToEnd();
}

TSharedPtr<class SWidget> UUW_Chat::GetChatInputTextObejct()
{
	return TB_SendMessage->GetCachedWidget();
}

void UUW_Chat::OnChatMessageCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	APC_Player* PC = Cast<APC_Player>(GetOwningPlayer());
	if (!PC)
		return;

	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
		if (!Text.IsEmpty())
		{
			PC->SendChatMessage(Text);
			TB_SendMessage->SetText(FText::GetEmpty());
		}
		PC->FocusGameMode(0);
		break;
	case ETextCommit::OnCleared:
		PC->FocusGameMode(0);
		break;
	}
}
