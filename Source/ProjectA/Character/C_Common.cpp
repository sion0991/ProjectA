// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Common.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "Abilities/RPGAttributeSet.h"
#include "Abilities/RPGGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/CharacterDataAsset.h"
#include "Gameplay/PS_PlayerState.h"
#include "Character/EquipItem.h"

AC_Common::AC_Common(const class FObjectInitializer& ObjectInitializer)//:
	//Super(ObjectInitializer.SetDefaultSubobjectClass<UGDCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	/*PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	bReplicates = true;
	bAlwaysRelevant = true;*/

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MainWeapon = CreateDefaultSubobject<UEquipItem>(TEXT("MainWeapon"));
	MainWeapon->SetupAttachment(GetMesh());

	SubWeapon = CreateDefaultSubobject<UEquipItem>(TEXT("SubWeapon"));
	SubWeapon->SetupAttachment(GetMesh());

	InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCollisionProfileName(FName("Interact"));
	EnableInput(Cast<APlayerController>(GetController()));


	//isCanAction = true;
	//isCanMove = true;
	//isStun = false;
	//isEquip = false;
	//isGuard = false;
	//isWalk = false;
}



UAbilitySystemComponent* AC_Common::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

int32 AC_Common::GetAbilityLevel(ERPGAbilityInputID AbilityID) const
{
	return 1;
}

void AC_Common::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

ERPGHitReactDirection AC_Common::GetHitReactDirection(const FVector& ImpactPoint)
{
	return ERPGHitReactDirection();
}

void AC_Common::PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser)
{
}

bool AC_Common::PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser)
{
	return false;
}

bool AC_Common::IsAlive() const
{
	//return GetHP() > 0.0;
	return true;
}

void AC_Common::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	/*if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}*/
}

void AC_Common::FinishDying()
{
	Destroy();
}

void AC_Common::Parried(float Duration)
{
	Stun(Duration);
}

void AC_Common::Stun(float Duration)
{ 
}

void AC_Common::Finishable()
{
	/*if (isCanAction)
	{
		if (Target)
		{
			isCanAction = false;
			PlayAnimMontage(CharacterData->AnimMontages[20]);
			FRotator rot = FRotator(0, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Yaw, 0);
			float Dist = 1;
			FVector Loc = Target->GetActorLocation() + (rot.Vector() * Dist);
			SetActorTransform(FTransform(rot, Loc, FVector3d::OneVector));
			ExecuteFinisherVictim(Cast<ACharacter>(Target), 20);
		}
	}*/
}

void AC_Common::ExecuteFinisherVictim(ACharacter* _Target, int FinisherIndex)
{
	PlayAnimMontage(CharacterData->AnimMontages[FinisherIndex + 1]);
	FRotator rot = FRotator(0, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Yaw, 0);
	_Target->SetActorRotation(rot);
}

void AC_Common::SetMoveSpeed()
{
	//GetCharacterMovement()->MaxWalkSpeed = isWalk ? WalkSpeed : RunSpeed;
}

void AC_Common::ShotArrow()
{
}

void AC_Common::InputAbilitySystemInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void AC_Common::InputAbilitySystemInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void AC_Common::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	for (auto RPGAbility : CharacterAbilities)
	{
		if (!RPGAbility)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player CharacterAbilities Empty"));
			break;
		}
		URPGGameplayAbility* AbilityCDO = RPGAbility->GetDefaultObject<URPGGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1);
		AbilitySpec.SourceObject = this;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityCDO->AbilityInputTag);

		/*const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
		AbilitySpecHandles.Add(AbilitySpecHandle);*/
		//AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityCDO, 1, FCString::Atoi(*AbilityCDO->Ability.GetTagName().ToString()), this));
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AC_Common::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AC_Common::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 
			GetCharacterLevel(), EffectContext);

		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = 
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void AC_Common::SetHP(float HP)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHP(HP);
	}
}

void AC_Common::SetMP(float MP)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetMP(MP);
	}
}

void AC_Common::SetAP(float AP)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetAP(AP);
	}
}

int32 AC_Common::GetCharacterLevel() const
{
	return AttributeSet.IsValid() ?
		static_cast<int32>(AttributeSet->GetCharacterLevel()) : -1;
}

float AC_Common::GetHP() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetHP() : -1;
}

float AC_Common::GetMaxHP() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetMaxHP() : -1;
}

float AC_Common::GetMP() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetMP() : -1;
}

float AC_Common::GetMaxMP() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetMaxMP() : -1;
}

float AC_Common::GetAP() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetAP() : -1;
}

float AC_Common::GetMaxAP() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetMaxAP() : -1;
}

float AC_Common::GetMoveSpeed() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetMoveSpeed() : -1;
}

float AC_Common::GetMoveSpeedBaseValue() const
{
	return AttributeSet.IsValid() ?
		AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet.Get())->GetBaseValue() : -1;
}