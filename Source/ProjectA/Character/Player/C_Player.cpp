// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "RPGEnhancedInputComponent.h"
#include "Data/InventorySystemComponent.h"
#include "Data/ST_Item.h"
#include "Data/CharacterDataAsset.h"
#include "Util/DataFunctionLibrary.h"
#include "I_PlayerAnim.h"
#include "Gameplay/PC_Player.h"
#include "Gameplay/PS_PlayerState.h"
#include "Character/State/StateBase.h"
#include "Object/ItemObject.h"
#include "../EquipItem.h"

AC_Player::AC_Player(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeRotation(FRotator(0, 270, 0));

	if (CharacterData)
	{
		GetMesh()->SetSkeletalMesh(CharacterData->Mesh);
		GetMesh()->SetAnimInstanceClass(CharacterData->AnimInstance);
	}
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	OverrideInputComponentClass = URPGEnhancedInputComponent::StaticClass();

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AC_Player::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AC_Player::OnOverlapEnd);

}

//서버와 클라이언트에서 초기화
void AC_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APS_PlayerState* PS = GetPlayerState<APS_PlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		InventorySystemComponent = Cast<UInventorySystemComponent>(PS->GetInventorySystemComponent());
		AttributeSet = PS->GetAttributeSet();
		AddCharacterAbilities();
	}
	ChangeState(EStateType::Common);
}

void AC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bLockOnEngaged ? EngageLock() : 0;
}

void AC_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	URPGEnhancedInputComponent* PEI = Cast<URPGEnhancedInputComponent>(PlayerInputComponent);
	if (PEI)
	{
		PEI->BindAction(InputConfig->OtherInputActions[0], ETriggerEvent::Triggered, this, &AC_Player::Move);
		PEI->BindAction(InputConfig->OtherInputActions[1], ETriggerEvent::Triggered, this, &AC_Player::Look);
		//PEI->BindAction(InputConfig->OtherInputActions[2], ETriggerEvent::Started, this, &AC_Player::Interact);
		PEI->BindAction(InputConfig->OtherInputActions[2], ETriggerEvent::Started, this, &AC_Player::IronSight);
		PEI->BindAction(InputConfig->OtherInputActions[3], ETriggerEvent::Started, this, &AC_Player::LockOnTarget);
		TArray<uint32> BindHandles;
		PEI->BindAbilityActions(InputConfig, this, &AC_Player::InputAbilitySystemInputTagPressed, &AC_Player::InputAbilitySystemInputTagReleased, BindHandles);
	}
	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &AC_Player::Walk);
}

void AC_Player::OnRep_PlayerState()
{
	APS_PlayerState* PS = GetPlayerState<APS_PlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		InventorySystemComponent = Cast<UInventorySystemComponent>(PS->GetInventorySystemComponent());
	}
}


float AC_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//if (isGuard)
	//{
	//	if (ParryTime - GetWorld()->GetUnpausedTimeSeconds() <= 0)
	//	{
	//		FString str = FString::FromInt(FDamageEvent::ClassID);
	//		str == "DT_LightAttack" ?
	//			true : false;

	//		//어떻게할지고민
	//		/*switch (str)
	//		{
	//		case L'DT_LightAttack':
	//			break;
	//		case L'DT_HeavyAttack':
	//			break;
	//		case L'DT_GuardBreakAttack':
	//			break;
	//		case L'DT_CantGuardAttack':
	//			break;
	//		}*/
	//	}
	//}
	//else
	//{

	//}
	//return 0.0f;
	return 0;
}

void AC_Player::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UI_Interactable::StaticClass()))
	{
		Cast<II_Interactable>(OtherActor)->SetWidgetVisibility(true);
		GetPlayerController()->OnInteract(Cast<II_Interactable>(OtherActor)->GetInteractableType(), true);
		InteractableObject = OtherActor;
		InteractNormal = SweepResult.Normal + FRotator(0, 180, 0).Vector();
	}	
}

void AC_Player::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == InteractableObject)
	{
		Cast<II_Interactable>(OtherActor)->SetWidgetVisibility(false);
		GetPlayerController()->OnInteract(Cast<II_Interactable>(InteractableObject)->GetInteractableType(), false);
		InteractableObject = nullptr;
	}	
}

void AC_Player::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	CurrentState->InputMove(MovementVector);
}

void AC_Player::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	CurrentState->InputLook(LookAxisVector);
}

void AC_Player::ShotArrow()
{
	FVector Start = GetMainWeapon()->GetSocketTransform("BowSocket").GetLocation();
	FVector LookTarget = Camera->GetComponentLocation() + Camera->GetForwardVector() * 1000.f;

	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(Start, LookTarget);

	GetWorld()->SpawnActor<AActor>(DataFunctionLibrary::GetSpawnActor(0), FTransform(LookRot, Start, FVector(2,2,2)));
}

void AC_Player::LockOnTarget()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> Types;	
	Types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); //Types.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 2000.0f, Types, ACharacter::StaticClass(), IgnoreActors, LockOnTargets);
	if (!LockOnTargets.IsEmpty())
	{
		Target = LockOnTargets[LockOnIndex];
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		//bLockOnEngaged = true;
		float time = GetWorld()->GetDeltaSeconds();
		FTimerDelegate ca;
		ca.BindUFunction(this, FName("EngageLock"));
		GetWorld()->GetTimerManager().SetTimer(LockOnHandle, ca, time, true);
	}
}

void AC_Player::DisengageLock()
{
	bLockOnEngaged = false;
	LockOnIndex = 0;
}

void AC_Player::EngageLock()
{
	float time = GetWorld()->GetDeltaSeconds();
	LockDelegateTime += time;
	FRotator LookAtTargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	FRotator InterpTargetRot = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(), LookAtTargetRot, time, LockOnSpeed);
	//FRotator Rot = FRotator(InterpTargetRot.Pitch, InterpTargetRot.Yaw, GetActorRotation().Roll);
	//GetController()->SetControlRotation(Rot);
	GetController()->SetControlRotation(InterpTargetRot);
	if (LockDelegateTime >= 3)
	{
		GetWorld()->GetTimerManager().ClearTimer(LockOnHandle);
		LockDelegateTime = 0;
	}
}

void AC_Player::SwapLock()
{

}

void AC_Player::Guard()
{
	/*if (isEquip && GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		ParryTime = GetWorld()->GetUnpausedTimeSeconds();

		PlayAnimMontage(CharacterData->AnimMontages[2],
			1.0f,
			DataFunctionLibrary::GetEnumDisplayNameToName(MyController->GetInventorySystemComponent()->GetEquipItem(0)->WeaponType);
		isGuard = true;
	}
	SetMoveSpeed();*/
}

void AC_Player::UnGuard()
{
	if (GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == CharacterData->AnimMontages[2])
	{
		StopAnimMontage(CharacterData->AnimMontages[2]);
	}
	//isGuard = false;
	SetMoveSpeed();
}

void AC_Player::Walk()
{
	//isWalk = !isWalk;
	SetMoveSpeed();
}

void AC_Player::IronSight()
{

}

void AC_Player::Jump()
{
	Super::Jump();

	CurrentState->InputJump();
}

void AC_Player::ClimbWall()
{
	GetCharacterMovement()->Velocity = FVector::Zero();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	SetActorRotation(InteractNormal.Rotation());
	ChangeState(EStateType::Climb);
}

void AC_Player::Interact()
{
	if (UKismetSystemLibrary::DoesImplementInterface(InteractableObject, UI_Interactable::StaticClass()))
	{
		Cast<II_Interactable>(InteractableObject)->InteractEvent(this);
	}
}

void AC_Player::AddItem(FST_Item* ItemInfo)
{
	InventorySystemComponent.Get()->AddItem(*ItemInfo);
}
void AC_Player::Riding()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->Velocity = FVector3d::ZeroVector;
	DisableInput(GetPlayerController());
	GetMesh()->GetAnimInstance()->Montage_Play(CharacterData->AnimMontages[7]);
	GetAnimInterface()->Execute_SetIsRide(GetMesh()->GetAnimInstance(), true);
	ChangeState(EStateType::Riding);
}
void AC_Player::UnRiding()
{
	GetMesh()->GetAnimInstance()->Montage_Play(CharacterData->AnimMontages[8]);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetAnimInterface()->Execute_SetIsRide(GetMesh()->GetAnimInstance(),false);
	ChangeState(EStateType::Common);
}

void AC_Player::TakeOffEquipment(int UW_InventorySlotIndex, int EquipSlotIndex)
{
	GetPlayerController()->GetInventorySystemComponent()->DropUnEquipItem(UW_InventorySlotIndex, EquipSlotIndex);
	ShowEquipment(EquipSlotIndex);
	//Stat->CalcItemStat(MyController->GetInventorySystemComponent()->GetEquipItemStat());
}

void AC_Player::ShowEquipment(int EquipSlotIndex)
{
	CurrentState->InputChangeEquip(GetPlayerController()->GetInventorySystemComponent()->GetEquipItem(EquipSlotIndex),
		EquipSlotIndex == 0 ? GetMainWeapon() : GetSubWeapon());
}

APC_Player* AC_Player::GetPlayerController()
{
	return Cast<APC_Player>(GetController());
}

II_PlayerAnim* AC_Player::GetAnimInterface()
{
	return Cast<II_PlayerAnim>(GetMesh()->GetAnimInstance());
}