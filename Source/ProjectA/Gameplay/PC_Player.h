// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PC_Player.generated.h"

class UInventorySystemComponent;
class APlayerHUD;
class APS_PlayerState;
class AC_Player;
class UInputMappingContext;
class UInputConfig;
struct FGameplayTag;
struct FST_Item;

UCLASS()
class PROJECTA_API APC_Player : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parts, meta = (AllowPrivateAccess = "true"))
	UInputConfig* InputConfig;

protected:
	TWeakObjectPtr<UInventorySystemComponent> InventorySystemComponent;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;
	virtual void OnUnPossess() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRep_PlayerState() override;
	virtual void AttachToPawn(APawn* InPawn) override;
public:
	APC_Player();
	virtual void SetupInputComponent() override;

	class UInventorySystemComponent* GetInventorySystemComponent() const;
	void InputAbilitySystemInputTagPressed(FGameplayTag InputTag);
	void InputAbilitySystemInputTagReleased(FGameplayTag InputTag);

	void ShowDamageNumber(float DamageAmount, AC_Player* TargetCharacter);
	void SetRespawnCountdown(float RespawnTimeRemaining);

private:
	UFUNCTION(Server, UnReliable, WithValidation)
	void CToS_SendMessage(const FString& Message);
	void CToS_SendMessage_Implementation(const FString& Message);
	bool CToS_SendMessage_Validate(const FString& Message);

	UFUNCTION(Client, UnReliable)
	void SToC_SendMessage(const FString& Message);
	void SToC_SendMessage_Implementation(const FString& Message);

public:
	void OnInventory();
	void OnInteract(int Type, bool OnOff);

	void SendChatMessage(const FText& Message);
	void FocusGameMode(int mode);
	void FocusChatInputText();

	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void DropItem(int index);
	void UseItem(int InventorySlotIndex, int EquipSlotIndex);
	void LoadData();
	FName GetWeaponType();
	void SwitchPossess(APawn* NewPawn);

	APlayerHUD* GetPlayerHUD();
};
