// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Attack.h"
#include "Character/Player/C_Player.h"
#include "Gameplay/PC_Player.h"
#include "Data/CharacterDataAsset.h"
#include "../AT_PlayMontageAndWaitForEvent.h"

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		AC_Player* Character = CastChecked<AC_Player>(ActorInfo->AvatarActor.Get());
		
		GetWorld()->GetTimerManager().ClearTimer(AttackHandle);

		UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, Character->GetCharacterData()->AnimMontages[MontageNum[CurrentMontageNum]], AbilityTags, 1.f, Character->GetPlayerController()->GetWeaponType());
		//UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, Character->GetCharacterData()->AnimMontages[CurrentMontageNum], AbilityTags);
		Task->OnBlendOut.AddDynamic(this, &UGA_Attack::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &UGA_Attack::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &UGA_Attack::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &UGA_Attack::OnCancelled);
		Task->EventReceived.AddDynamic(this, &UGA_Attack::EventReceived);
		// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
		Task->ReadyForActivation();
	}
}

bool UGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AC_Common* Character = CastChecked<AC_Common>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	return !Character->GetMovementComponent()->IsFalling();
}

void UGA_Attack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	/*if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}*/
}

void UGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Attack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Attack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (CurrentMontageNum + 1 >= MontageNum.Num())
	{
		CurrentMontageNum = 0;
	}
	else
	{
		CurrentMontageNum++;

		FTimerDelegate timerCallback;
		timerCallback.BindLambda([&] {
			AC_Common* Character = Cast<AC_Common>(GetAvatarActorFromActorInfo());
			if (CurrentMontageNum != 0)
			{
				if (GetCurrentMontage() != Character->GetCharacterData()->AnimMontages[MontageNum[CurrentMontageNum]] ||
					GetCurrentMontage() != Character->GetCharacterData()->AnimMontages[MontageNum[CurrentMontageNum - 1]] ||
					!GetCurrentMontage())
					CurrentMontageNum = 0;
			}
		});

		GetWorld()->GetTimerManager().SetTimer(AttackHandle, timerCallback, 0.35f, false);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Attack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
