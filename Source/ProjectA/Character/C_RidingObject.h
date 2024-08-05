// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Object/I_Interactable.h"
#include "Character/I_Character.h"
#include "C_RidingObject.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTA_API AC_RidingObject : public ACharacter, public II_Interactable
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RidingCollision;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class UCharacterDataAsset* CharacterData;
	UPROPERTY(EditAnywhere)
	class UInputConfig* InputConfig;
	class AC_Player* Rider;

	float RunSpeed = 900;
	float SprintSpeed = 1300;
	float WalkSpeed = 400;

	int InteractType = 2;
public:
	AC_RidingObject();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;
public:
	virtual void InteractEvent(AC_Player* Player) override;
	virtual void SetWidgetVisibility(bool IsVisible) override {}
	virtual int GetInteractableType() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Sprint();
	void EndSprint();
	void UnRiding();
	virtual void Jump() override;
	virtual void ResetJumpState() override;
};
