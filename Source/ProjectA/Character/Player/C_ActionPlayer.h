// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "C_ActionPlayer.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walking		UMETA(DisplayName ="Walking"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
	Crouching	UMETA(DisplayName = "Crouching"),
	Sliding		UMETA(DisplayName = "Sliding")
};

UCLASS(Blueprintable, BlueprintType)
class PROJECTA_API AC_ActionPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UInputConfig* InputConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UCharacterDataAsset* CharacterData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SlidingTimelineCurve;
	FTimeline SlidingTimeline;
	FOnTimelineFloat SlidingTimelineUpdateDelegate;
	FOnTimelineEvent SlidingTimelineFinishDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* CrouchTimelineCurve;
	FTimeline CrouchTimeline;
	FOnTimelineFloat CrouchTimelineUpdateDelegate;
	FOnTimelineEvent CrouchTimelineFinishDelegate;

	EMovementState MoveState;
	bool IsSprintKeyDown;
	bool IsCrouchKeyDown;
	float StandingCapsuleHalfHeight;
	float StandingCameraZOffset;

	float WalkSpeed;
	float SprintSpeed;
	float CrouchSpeed;
	float SlidingForce;
	float FloorSpeed;

public:
	// Sets default values for this character's properties
	AC_ActionPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetFloorChanged(int FloorType);
private:
	void InputMove(const FInputActionValue& Value);
	void InputLook(const FInputActionValue& Value);
	void InputSprint();
	void ReleaseSprint();
	void InputCrouch();
	void ReleaseCrouch();
	void BeginCrouch();
	void EndCrouch();
	void BeginSlide();
	void EndSlide();
	void BeginCameraTilt();
	void EndCameraTilt();

	void ResolveMovementState();
	void SetMovementState(EMovementState NewMoveState);
	void OnMovementStateChanged(EMovementState PrevMoveState);
	FVector CalculateFloorInfluence(FVector FloorNormal);
	bool CanSprint();
	bool CanStand();

	UFUNCTION()
	void CrouchTimelineUpdate(float Output);
	UFUNCTION()
	void CrouchTimelineFinish();
	UFUNCTION()
	void SlidingTimelineUpdate(float Output);
	UFUNCTION()
	void SlidingTimelineFinish();
	void StandingFloorChanged();
};
