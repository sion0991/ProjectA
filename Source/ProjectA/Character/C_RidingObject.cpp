// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/C_RidingObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Character/Player/C_Player.h"
#include "Gameplay/PC_Player.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/CharacterDataAsset.h"
#include "Data/InputConfig.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

AC_RidingObject::AC_RidingObject()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	bReplicates = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bAlwaysRelevant = true;

	RidingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RidingCollision"));
	RidingCollision->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetBoxExtent(FVector(30, 130, 88));
	BoxCollision->SetCollisionProfileName(TEXT("BlockAll"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500;
	CameraBoom->SocketOffset = FVector(0, 0, 100);
	CameraBoom->SetRelativeRotation(FRotator(-20, 0, 0));
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	if (CharacterData)
	{
		GetMesh()->SetSkeletalMesh(CharacterData->Mesh);
		GetMesh()->SetAnimInstanceClass(CharacterData->AnimInstance);
	}

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 230.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	OverrideInputComponentClass = UEnhancedInputComponent::StaticClass();
}

void AC_RidingObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (PEI)
	{
		PEI->BindAction(InputConfig->OtherInputActions[0], ETriggerEvent::Triggered, this, &AC_RidingObject::Move);
		PEI->BindAction(InputConfig->OtherInputActions[1], ETriggerEvent::Triggered, this, &AC_RidingObject::Look);
		PEI->BindAction(InputConfig->OtherInputActions[2], ETriggerEvent::Started, this, &AC_RidingObject::Sprint);
		PEI->BindAction(InputConfig->OtherInputActions[2], ETriggerEvent::Completed, this, &AC_RidingObject::EndSprint);
		PEI->BindAction(InputConfig->OtherInputActions[3], ETriggerEvent::Started, this, &AC_RidingObject::Jump);
		PEI->BindAction(InputConfig->OtherInputActions[4], ETriggerEvent::Started, this, &AC_RidingObject::UnRiding);
	}
}
void AC_RidingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AC_RidingObject::InteractEvent(AC_Player* Player)
{
	Rider = Player;
	GetMesh()->GetAnimInstance()->Montage_Play(CharacterData->AnimMontages[0]);
	Rider->Riding();
}

int AC_RidingObject::GetInteractableType()
{
	return InteractType;
}

void AC_RidingObject::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	
	const FRotator YawRotation(0,GetController()->GetControlRotation().Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);

	if (!GetMovementComponent()->IsFalling())
		AddControllerYawInput(GetWorld()->GetDeltaSeconds() * 40.0f * MoveVector.X);
}

void AC_RidingObject::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (GetVelocity().Length() < 200 || GetMovementComponent()->IsFalling())
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	AddControllerPitchInput(LookAxisVector.Y);
}

void AC_RidingObject::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AC_RidingObject::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AC_RidingObject::UnRiding()
{
	if (GetCurrentMontage() || GetMovementComponent()->IsFalling())
		return;
	Rider->UnRiding();
	
}

void AC_RidingObject::Jump()
{
	if (GetMovementComponent()->IsFalling())
		return;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	Super::Jump();
	float Dir = (UKismetMathLibrary::MakeRotFromX(GetLastMovementInputVector()) - GetControlRotation()).Yaw;
	AddActorWorldRotation(FRotator(0, Dir, 0));
}

void AC_RidingObject::ResetJumpState()
{
	Super::ResetJumpState();
	
	if(GetController())
	GetController()->SetControlRotation(FRotator(Controller->GetControlRotation().Pitch, GetActorRotation().Yaw, Controller->GetControlRotation().Roll));
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}