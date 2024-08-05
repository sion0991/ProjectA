// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Skill.h"
#include "Character/Player/C_Player.h"
#include "Data/CharacterDataAsset.h"
#include "../AT_PlayMontageAndWaitForEvent.h"
#include "Kismet/KismetMathLibrary.h"

UGA_Skill::UGA_Skill()
{
	//AbilityInputID = ERPGAbilityInputID::IA_None;
	/*AbilityInputID = ERPGAbilityInputID::IA_Sprint;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	AbilityInputTag = (FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	CooldownGameplayEffectClass = UGE_TSkill1Cooldown::StaticClass();*/
}

void UGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		AC_Common* Character = CastChecked<AC_Common>(ActorInfo->AvatarActor.Get());
		if (!Character->GetLastMovementInputVector().Equals(FVector3d::Zero(), 0.0001f))
		{
			Character->SetActorRotation(UKismetMathLibrary::MakeRotFromX(Character->GetLastMovementInputVector()));
		}
		UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, Character->GetCharacterData()->AnimMontages[MontageNum], AbilityTags);
		Task->OnBlendOut.AddDynamic(this, &UGA_Skill::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &UGA_Skill::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &UGA_Skill::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &UGA_Skill::OnCancelled);
		Task->EventReceived.AddDynamic(this, &UGA_Skill::EventReceived);
		// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
		Task->ReadyForActivation();
	}
}

bool UGA_Skill::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AC_Common* Character = CastChecked<AC_Common>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	return !Character->GetCurrentMontage() && !Character->GetMovementComponent()->IsFalling() && MontageNum >= 0;
}

void UGA_Skill::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	/*if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}*/
}

// Epic's comment
/**
 *	Canceling an non instanced ability is tricky. Right now this works for Jump since there is nothing that can go wrong by calling
 *	StopJumping() if you aren't already jumping. If we had a montage playing non instanced ability, it would need to make sure the
 *	Montage that *it* played was still playing, and if so, to cancel it. If this is something we need to support, we may need some
 *	light weight data structure to represent 'non intanced abilities in action' with a way to cancel/end them.
 */
void UGA_Skill::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGA_Skill::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Skill::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Skill::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Skill::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		AC_Common* Hero = Cast<AC_Common>(GetAvatarActorFromActorInfo());
		if (!Hero)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
}
