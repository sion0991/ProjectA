// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Guard.h"
#include "Character/Player/C_Player.h"
#include "Gameplay/PC_Player.h"
#include "Data/CharacterDataAsset.h"
#include "../AT_PlayMontageAndWaitForEvent.h"

UGA_Guard::UGA_Guard()
{
	//AbilityInputID = ERPGAbilityInputID::IA_None;
	/*AbilityInputID = ERPGAbilityInputID::IA_Sprint;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	AbilityInputTag = (FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Sprint")));
	CooldownGameplayEffectClass = UGE_TSkill1Cooldown::StaticClass();*/
}

void UGA_Guard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		AC_Player* Character = CastChecked<AC_Player>(ActorInfo->AvatarActor.Get());
		UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, Character->GetCharacterData()->AnimMontages[MontageNum], AbilityTags, 1.f, Character->GetPlayerController()->GetWeaponType());
		Task->OnBlendOut.AddDynamic(this, &UGA_Guard::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &UGA_Guard::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &UGA_Guard::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &UGA_Guard::OnCancelled);
		Task->EventReceived.AddDynamic(this, &UGA_Guard::EventReceived);
		// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
		Task->ReadyForActivation();
	}
}

bool UGA_Guard::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AC_Common* Character = CastChecked<AC_Common>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	return !Character->GetCurrentMontage() && !Character->GetMovementComponent()->IsFalling() && MontageNum >= 0;
}

void UGA_Guard::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	/*if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}*/
	AC_Common* Character = CastChecked<AC_Common>(ActorInfo->AvatarActor.Get());
	Character->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == Character->GetCharacterData()->AnimMontages[MontageNum] ?
		Character->StopAnimMontage(Character->GetCharacterData()->AnimMontages[MontageNum]) : 0;
}

// Epic's comment
/**
 *	Canceling an non instanced ability is tricky. Right now this works for Jump since there is nothing that can go wrong by calling
 *	StopJumping() if you aren't already jumping. If we had a montage playing non instanced ability, it would need to make sure the
 *	Montage that *it* played was still playing, and if so, to cancel it. If this is something we need to support, we may need some
 *	light weight data structure to represent 'non intanced abilities in action' with a way to cancel/end them.
 */
void UGA_Guard::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGA_Guard::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Guard::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Guard::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Guard::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
