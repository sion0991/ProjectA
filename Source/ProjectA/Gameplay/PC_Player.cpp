// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Player.h"
#include "Character/Player/C_Player.h"
#include "Character/I_Character.h"
#include "PCM_Player.h"
#include "PS_PlayerState.h"
#include "PlayerHUD.h"
#include "Util/DataFunctionLibrary.h"
#include "Data/CharacterDataAsset.h"
#include "Data/InventorySystemComponent.h"
#include "Data/ST_Item.h"
#include "Data/SG_PlayerData.h"
#include "Components/InputComponent.h"
#include "Character/Player/RPGEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTask.h"

APC_Player::APC_Player()
{
	PlayerCameraManagerClass = APCM_Player::StaticClass();
}

void APC_Player::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	APS_PlayerState* PS = GetPlayerState<APS_PlayerState>();
	if (PS)
	{
		InventorySystemComponent = Cast<UInventorySystemComponent>(PS->GetInventorySystemComponent());
		//LoadData();
	}
		
}

void APC_Player::BeginPlay()
{
	APlayerHUD* PlayerHUD = GetPlayerHUD();
}

void APC_Player::OnUnPossess()
{
	//DataFunctionLibrary::SaveGame(this, "Data", 0);
	Super::OnUnPossess();
}

void APC_Player::OnRep_PlayerState()
{
	APS_PlayerState* PS = GetPlayerState<APS_PlayerState>();
	if (PS)
	{
		InventorySystemComponent = Cast<UInventorySystemComponent>(PS->GetInventorySystemComponent());
		//LoadData();
	}
}

void APC_Player::AttachToPawn(APawn* InPawn)
{
	if (RootComponent)
	{
		if (InPawn)
		{
			// Only attach if not already attached.
			if (InPawn->GetRootComponent() && RootComponent->GetAttachParent() != InPawn->GetRootComponent())
			{
				RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				RootComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
				RootComponent->AttachToComponent(InPawn->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}
		else
		{
			DetachFromPawn();
		}
	}
}

void APC_Player::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	InputComponent = NewObject<URPGEnhancedInputComponent>(URPGEnhancedInputComponent::StaticClass());

	if (URPGEnhancedInputComponent* const EnhancedInputComponent = Cast<URPGEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(InputConfig->OtherInputActions[0], ETriggerEvent::Started, this, &APC_Player::FocusChatInputText);
		EnhancedInputComponent->BindAction(InputConfig->OtherInputActions[1], ETriggerEvent::Started, this, &APC_Player::OnInventory);
	}
}

UInventorySystemComponent* APC_Player::GetInventorySystemComponent() const
{
	return InventorySystemComponent.Get();
}

void APC_Player::InputAbilitySystemInputTagPressed(FGameplayTag InputTag)
{
	Cast<AC_Player>(GetPawnOrSpectator())->InputAbilitySystemInputTagPressed(InputTag);
}

void APC_Player::InputAbilitySystemInputTagReleased(FGameplayTag InputTag)
{
	Cast<AC_Player>(GetPawnOrSpectator())->InputAbilitySystemInputTagReleased(InputTag);
}

void APC_Player::ShowDamageNumber(float DamageAmount, AC_Player* TargetCharacter)
{
}

void APC_Player::SetRespawnCountdown(float RespawnTimeRemaining)
{
}
void APC_Player::OnInventory()
{
	if (!GetPlayerHUD())
		return;

	GetPlayerHUD()->OnOffInventory();
}

void APC_Player::OnInteract(int Type, bool OnOff)
{
	//Type : 0 = ��ȣ�ۿ������Ʈ, 1 = ������
	if(GetPlayerHUD())
	GetPlayerHUD()->OnOffInteract(Type, OnOff);
}

//void APC_Player::InputStatus()
//{
//	if (MyMainHUD->StatVisible == ESlateVisibility::Visible)
//	{
//		MyMainHUD->StatVisible = ESlateVisibility::Hidden;
//		bShowMouseCursor = false;
//		SetInputMode(FInputModeGameOnly());
//	}
//	else
//	{
//		MyMainHUD->StatVisible = ESlateVisibility::Visible;
//		bShowMouseCursor = true;
//		SetInputMode(FInputModeGameAndUI());
//	}
//}



void APC_Player::SendChatMessage(const FText& Message)
{
	//�����̸� �������� �߰�
	//FString UserName = PC->GetUserName();
	//FString OutMessage = FString::Printf(TEXT"%s : %s"), *UserName, *Message.ToString();
	CToS_SendMessage(Message.ToString());
}
void APC_Player::FocusGameMode(int mode)
{
	switch (mode)
	{
	case 0:
		SetInputMode(FInputModeGameOnly());
		break;
	case 1:
		SetInputMode(FInputModeUIOnly());
		break;
	case 2:
		SetInputMode(FInputModeGameAndUI());
		break;
	}
}

void APC_Player::FocusChatInputText()
{
	if (!GetPlayerHUD())
		return;

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(GetPlayerHUD()->GetChatInputTextObejct());

	SetInputMode(InputMode);
}

void APC_Player::CToS_SendMessage_Implementation(const FString& Message)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), 
		APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		APC_Player* PC = Cast<APC_Player>(OutActor);
		if (PC)
			PC->SToC_SendMessage(Message);
	}
}

bool APC_Player::CToS_SendMessage_Validate(const FString& Message)
{
	if (Message.Len() < 255)
		return true;
	return false;
}

void APC_Player::SToC_SendMessage_Implementation(const FString& Message)
{
	GetPlayerHUD()->AddChatMessage(Message);
}

void APC_Player::UseItem(int InventorySlotIndex, int EquipSlotIndex)
{
	bool isEquip;
	InventorySystemComponent->UseItem(InventorySlotIndex, EquipSlotIndex, isEquip);
	if (isEquip)
	{
		AC_Player* CPlayer = Cast<AC_Player>(GetPawnOrSpectator()); 
		CPlayer->ShowEquipment(EquipSlotIndex);
	}
	else
	{
		FST_Item* Item = InventorySystemComponent->GetInventoryItem(InventorySlotIndex);
		APS_PlayerState* PS = GetPlayerState<APS_PlayerState>();
		if (Item->Duration == 0)
		{
			PS->UseConsumableItem(*Item);
		}
		else
		{
			PS->UseDurationItem(*Item);
		}
	}
}

void APC_Player::LoadData()
{
	USG_PlayerData* Data;

	DataFunctionLibrary::LoadGame("Data", 0, Data);
	if (Data->HasData)
	{
		GetInventorySystemComponent()->LoadInventory(Data);
	}
}

FName APC_Player::GetWeaponType()
{
	return DataFunctionLibrary::GetEnumDisplayNameToName(InventorySystemComponent->GetEquipItem(0)->WeaponType);
}

void APC_Player::SwitchPossess(APawn* NewPawn)
{
	Possess(NewPawn);
	NewPawn->EnableInput(this);
}

APlayerHUD* APC_Player::GetPlayerHUD()
{
	return Cast<APlayerHUD>(GetHUD());
}