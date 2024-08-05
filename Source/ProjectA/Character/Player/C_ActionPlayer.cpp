// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ActionPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "RPGEnhancedInputComponent.h"
#include "Util/DataFunctionLibrary.h"
#include "Data/CharacterDataAsset.h"
#include "Data/InputConfig.h"

// Sets default values
AC_ActionPlayer::AC_ActionPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;*/

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(GetMesh(), "head");
	Camera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeRotation(FRotator(0, 270, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	if (CharacterData)
	{
		GetMesh()->SetSkeletalMesh(CharacterData->Mesh);
		GetMesh()->SetAnimInstanceClass(CharacterData->AnimInstance);
	}

	StandingCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//StandingCameraZOffset = CameraBoom->SocketOffset.Z;

	OverrideInputComponentClass = URPGEnhancedInputComponent::StaticClass();

	WalkSpeed = 600;
	SprintSpeed = 1200;
	CrouchSpeed = 200;
	SlidingForce = 150000;
	FloorSpeed = 0;
	MoveState = EMovementState::Walking;
}

void AC_ActionPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (CrouchTimelineCurve)
	{
		CrouchTimelineUpdateDelegate.BindUFunction(this, FName("CrouchTimelineUpdate"));
		CrouchTimeline.AddInterpFloat(CrouchTimelineCurve, CrouchTimelineUpdateDelegate);

		CrouchTimelineFinishDelegate.BindUFunction(this, FName("CrouchTimelineFinish"));
		CrouchTimeline.SetTimelineFinishedFunc(CrouchTimelineFinishDelegate);

		float CrouchTimelineStart = 0;
		float CrouchTimelineLength = 0;
		CrouchTimelineCurve->GetTimeRange(CrouchTimelineStart, CrouchTimelineLength);
		CrouchTimeline.SetTimelineLength(CrouchTimelineLength);
	}
	if (SlidingTimelineCurve)
	{
		SlidingTimelineUpdateDelegate.BindUFunction(this, FName("SlidingTimelineUpdate"));
		SlidingTimeline.AddInterpFloat(SlidingTimelineCurve, SlidingTimelineUpdateDelegate);

		SlidingTimelineFinishDelegate.BindUFunction(this, FName("SlidingTimelineFinish"));
		SlidingTimeline.SetTimelineFinishedFunc(SlidingTimelineFinishDelegate);

		float SlidingTimelineStart = 0;
		float SlidingTimelineLength = 0;
		SlidingTimelineCurve->GetTimeRange(SlidingTimelineStart, SlidingTimelineLength);
		SlidingTimeline.SetTimelineLength(SlidingTimelineLength);
	}
}

void AC_ActionPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SlidingTimeline.TickTimeline(DeltaTime);
	CrouchTimeline.TickTimeline(DeltaTime);

	CanStand() ? ResolveMovementState() : 0;
}

void AC_ActionPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMapping, 0);
	}

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (PEI)
	{
		PEI->BindAction(InputConfig->OtherInputActions[0], ETriggerEvent::Triggered, this, &AC_ActionPlayer::InputMove);
		PEI->BindAction(InputConfig->OtherInputActions[1], ETriggerEvent::Triggered, this, &AC_ActionPlayer::InputLook);
		PEI->BindAction(InputConfig->OtherInputActions[6], ETriggerEvent::Started, this, &AC_ActionPlayer::InputSprint);
		PEI->BindAction(InputConfig->OtherInputActions[6], ETriggerEvent::Completed, this, &AC_ActionPlayer::ReleaseSprint);
		PEI->BindAction(InputConfig->OtherInputActions[7], ETriggerEvent::Started, this, &AC_ActionPlayer::InputCrouch);
		PEI->BindAction(InputConfig->OtherInputActions[7], ETriggerEvent::Completed, this, &AC_ActionPlayer::ReleaseCrouch);
	}
}

void AC_ActionPlayer::SetFloorChanged(int FloorType)
{
	FloorSpeed = FloorType;
	StandingFloorChanged();
}

void AC_ActionPlayer::InputMove(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	const FRotator YawRotation(0, GetController()->GetControlRotation().Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (MoveState != EMovementState::Sliding)
	{
		AddMovementInput(ForwardDirection, Movement.Y);
		AddMovementInput(RightDirection, Movement.X);
	}
}

void AC_ActionPlayer::InputLook(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AC_ActionPlayer::InputSprint()
{
	IsSprintKeyDown = true;
	switch (MoveState)
	{
	case EMovementState::Walking:
	case EMovementState::Crouching:
		SetMovementState(EMovementState::Sprinting);
		break;
	}
}

void AC_ActionPlayer::ReleaseSprint()
{
	IsSprintKeyDown = false;
	switch (MoveState)
	{
	case EMovementState::Walking:
		break;
	case EMovementState::Sprinting:
		ResolveMovementState();
		break;
	case EMovementState::Crouching:
		break;
	case EMovementState::Sliding:
		break;
	default:
		break;
	}
}

void AC_ActionPlayer::InputCrouch()
{
	IsCrouchKeyDown = true;
	switch (MoveState)
	{
	case EMovementState::Walking:
		SetMovementState(EMovementState::Crouching);
		break;
	case EMovementState::Sprinting:
		SetMovementState(EMovementState::Sliding);
		break;
	case EMovementState::Crouching:
		break;
	case EMovementState::Sliding:
		break;
	default:
		break;
	}
}

void AC_ActionPlayer::ReleaseCrouch()
{
	IsCrouchKeyDown = false;
}

void AC_ActionPlayer::BeginCrouch()
{
	CrouchTimeline.Play();
}

void AC_ActionPlayer::EndCrouch()
{
	CrouchTimeline.Reverse();
}

void AC_ActionPlayer::BeginSlide()
{
	BeginCameraTilt();
	SlidingTimeline.Play();
}

void AC_ActionPlayer::EndSlide()
{
	EndCameraTilt();
	SlidingTimeline.Stop();
}

void AC_ActionPlayer::BeginCameraTilt()
{
	//GetController()->SetControlRotation(FRotator(0, 0, -8));
}

void AC_ActionPlayer::EndCameraTilt()
{
	//GetController()->SetControlRotation(FRotator::ZeroRotator);
}

void AC_ActionPlayer::ResolveMovementState()
{
	EMovementState State = CanSprint() ? EMovementState::Sprinting : 
							CanStand() ? EMovementState::Walking : EMovementState::Crouching;

	SetMovementState(State);
}

void AC_ActionPlayer::SetMovementState(EMovementState NewMoveState)
{
	if (MoveState != NewMoveState)
	{
		EMovementState PrevMoveState = MoveState;
		MoveState = NewMoveState;
		OnMovementStateChanged(PrevMoveState);
		switch (MoveState)
		{
		case EMovementState::Walking:
		case EMovementState::Sprinting:
			EndCrouch();
			break;
		case EMovementState::Crouching:
			BeginCrouch();
			break;
		case EMovementState::Sliding:
			BeginCrouch();
			BeginSlide();
			break;
		}
	}
}

void AC_ActionPlayer::OnMovementStateChanged(EMovementState PrevMoveState)
{
	if (PrevMoveState == EMovementState::Sliding)
	{
		StandingFloorChanged();
	}
	switch (MoveState)
	{
	case EMovementState::Walking:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case EMovementState::Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		break;
	case EMovementState::Crouching:
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		break;
	case EMovementState::Sliding:
		GetCharacterMovement()->Velocity = GetActorForwardVector() * SprintSpeed;
		GetCharacterMovement()->GroundFriction = 0;
		GetCharacterMovement()->BrakingDecelerationWalking = 800;
		break;
	}
}

FVector AC_ActionPlayer::CalculateFloorInfluence(FVector FloorNormal)
{
	
	if (FVector::PointsAreNear(FloorNormal, FVector::UpVector, 0.0001f))
	{
		return FVector::Zero();
	}
	return FloorNormal.Cross(FloorNormal.Cross(FVector::UpVector)).GetSafeNormal(0.0001f);
}

bool AC_ActionPlayer::CanSprint()
{
	if (!IsSprintKeyDown)
		return false;

	return !GetCharacterMovement()->IsFalling() && CanStand();
}

bool AC_ActionPlayer::CanStand()
{
	if (IsCrouchKeyDown)
		return false;
	 
	FHitResult Result;
	ETraceTypeQuery TypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	TArray<AActor*> IgnoreActors;

	return !UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		GetActorLocation() + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		TypeQuery,
		false,
		IgnoreActors,
		EDrawDebugTrace::Type::None,
		Result,
		true
	);

	//GetActorLocation()- FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight())+

	//GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
}

void AC_ActionPlayer::CrouchTimelineUpdate(float Output)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::GetMappedRangeValueClamped(FVector2D(0, 1), FVector2D(35, StandingCapsuleHalfHeight), Output));
	/*Camera->SetRelativeLocation(FVector(Camera->GetRelativeLocation().X,
		Camera->GetRelativeLocation().Y,
		FMath::GetMappedRangeValueClamped(FVector2D(0, 1), FVector2D(25, StandingCameraZOffset), Output)));*/

}

void AC_ActionPlayer::CrouchTimelineFinish()
{
}

void AC_ActionPlayer::SlidingTimelineUpdate(float Output)
{
	GetCharacterMovement()->AddForce(CalculateFloorInfluence(GetCharacterMovement()->CurrentFloor.HitResult.Normal)*SlidingForce);
	if (GetVelocity().Length() > SprintSpeed)
	{
		GetCharacterMovement()->Velocity = GetVelocity().GetSafeNormal(0.0001f) * SprintSpeed;
	}

	if (GetVelocity().Length() < CrouchSpeed)
	{
		ResolveMovementState();
	}
	
}

void AC_ActionPlayer::SlidingTimelineFinish()
{
}

void AC_ActionPlayer::StandingFloorChanged()
{
	int Friction = FloorSpeed * 4;
	GetCharacterMovement()->GroundFriction = 8 - FMath::Clamp(Friction,0,8);
	GetCharacterMovement()->BrakingDecelerationWalking = 2048 - FloorSpeed * 500;
}