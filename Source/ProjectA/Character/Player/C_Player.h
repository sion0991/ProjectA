// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../C_Common.h"
#include "C_Player.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Common		UMETA(DisplayName = "Common"),
	Attack		UMETA(DisplayName = "Attack"),
	Climb		UMETA(DisplayName = "Climb"),
	RopeHanging	UMETA(DisplayName = "RopeHanging"),
	Riding		UMETA(DisplayName = "Riding")
};

class UStateBase;
class II_PlayerAnim;
class APC_Player;
class UCharacterStat;
class USpringArmComponent;
class UCameraComponent;
class AInteractable;
class UInputConfig;
struct FInputActionValue;

UCLASS(Blueprintable, BlueprintType)
class PROJECTA_API AC_Player : public AC_Common
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parts, meta = (AllowPrivateAccess = "true"))
		UInputConfig* InputConfig;

	//TArray<UStateBase*> UsedStates;
	bool bLockOnEngaged = false;
	float LockOnSpeed = 2.0f;
	int LockOnIndex = 0;
	float LockDelegateTime = 0;
	TArray<AActor*> LockOnTargets;
	FTimerHandle LockOnHandle;
	float ParryTime = 0;
	bool bAim = false;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStateBase* CurrentState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* InteractableObject;
	FVector InteractNormal;

public:
	AC_Player(const class FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;

	TWeakObjectPtr<class UInventorySystemComponent> InventorySystemComponent;
public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	virtual void ShotArrow() override;
	void LockOnTarget();
	void DisengageLock();
	UFUNCTION()
	void EngageLock();
	void SwapLock();
	void Guard();
	void UnGuard();
	void Walk();
	void IronSight();
	virtual void Jump() override;
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void ClimbWall();
	void Interact();

	void AddItem(struct FST_Item* ItemInfo);
	void Riding();
	void UnRiding();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "State")
	void ChangeState(EStateType State);
	void TakeOffEquipment(int UW_InventorySlotIndex, int EquipSlotIndex);
	void ShowEquipment(int EquipSlotIndex);
	bool GetIsAim() { return bAim; }
	USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	UCameraComponent* GetCamera() { return Camera; }
	APC_Player* GetPlayerController();
	II_PlayerAnim* GetAnimInterface();
};
