// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/I_PlayerAnim.h"
#include "Character/Player/RPGEnhancedInputComponent.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "Abilities/RPGGameplayAbility.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Data/InputConfig.h"
#include "Data/AbilityInputConfig.h"
#include "InputMappingContext.h"


// Sets default values
ATopDownCharacter::ATopDownCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,1080,0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1400.0f;
	SpringArm->SetRelativeRotation(FRotator(-25, 0, 0));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetUsingAbsoluteRotation(true);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	RWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RWeapon"));
	RWeapon->SetupAttachment(GetMesh());
	GetMesh()->SetRelativeRotation(FRotator(0, 270, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -95));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>SM(TEXT("SkeletalMesh'/Game/Player/Character/Mesh/SK_PlayerMq.SK_PlayerMq'"));
	if (SM.Succeeded())
		GetMesh()->SetSkeletalMesh(SM.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance>AINST(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/Character/Player/Anim/CPP_TopDownPlayer.CPP_TopDownPlayer_C'"));
	if (AINST.Succeeded())
		GetMesh()->SetAnimInstanceClass(AINST.Class);

	OverrideInputComponentClass = URPGEnhancedInputComponent::StaticClass();
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			/*for (auto p : Subsystem->GetPlayerInput()->ActionMappings)
			{
				p.ActionName
			}*/
		}
	}
	//AnimInterface = Cast<II_PlayerAnim>(GetMesh()->GetAnimInstance());
}
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called every frame
//void ATopDownCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (URPGEnhancedInputComponent* const EnhancedInputComponent = Cast<URPGEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCharacter::EndMove);
		TArray<uint32> BindHandles;
		EnhancedInputComponent->BindAbilityActions(InputConfig, this, &AC_Common::InputAbilitySystemInputTagPressed, &AC_Common::InputAbilitySystemInputTagReleased, BindHandles);
	}
}

void ATopDownCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
		AnimInterface->Execute_SetIsMove(GetMesh()->GetAnimInstance(), true);
	}
}

void ATopDownCharacter::EndMove()
{
	AnimInterface->Execute_SetIsMove(GetMesh()->GetAnimInstance(), false);
}

void ATopDownCharacter::Jump()
{
	//AnimInterface->Execute_Avoid(GetMesh()->GetAnimInstance());
}

void ATopDownCharacter::Attack()
{
	//AnimInterface->Execute_AttackCommon(GetMesh()->GetAnimInstance());
}


