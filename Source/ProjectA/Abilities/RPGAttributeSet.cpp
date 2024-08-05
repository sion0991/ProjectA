// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "../TopDown/TopDownCharacter.h"
#include "../TopDown/TopDownController.h"

URPGAttributeSet::URPGAttributeSet()
{
	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"));
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"));
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"));
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// �Ӽ��� ����� ������ ȣ��ǹǷ� �ִ� ü��/������ ���� ���� �հ踦 ��ġ�ϵ��� �����Ϸ��� ��
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHPAttribute())
		AdjustAttributeForMaxChange(HP, MaxHP, NewValue, GetHPAttribute());
	else if (Attribute == GetMaxMPAttribute())
	{
		AdjustAttributeForMaxChange(MP, MaxMP, NewValue, GetMPAttribute());
	}
	else if (Attribute == GetMaxAPAttribute())
	{
		AdjustAttributeForMaxChange(AP, MaxAP, NewValue, GetAPAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		// Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}

void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ATopDownCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ATopDownCharacter>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ATopDownCharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<ATopDownCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ATopDownCharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of Damage done and clear the Damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before Damage is added, handle Damage
			// This prevents Damage being added to dead things and replaying death animations
			bool WasAlive = true;

			if (TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();
			}

			if (!TargetCharacter->IsAlive())
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving Damage"), TEXT(__FUNCTION__), *TargetCharacter->GetName());
			}

			// Apply the HP change and then clamp it
			const float NewHP = GetHP() - LocalDamageDone;
			SetHP(FMath::Clamp(NewHP, 0.0f, GetMaxHP()));

			if (TargetCharacter && WasAlive)
			{
				// This is the log statement for Damage received. Turned off for live games.
				//UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), TEXT(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

				// Play HitReact animation and sound with a multicast RPC.
				const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();

				if (Hit)
				{
					ERPGHitReactDirection HitDirection = TargetCharacter->GetHitReactDirection(Data.EffectSpec.GetContext().GetHitResult()->Location);
					switch (HitDirection)
					{
					case ERPGHitReactDirection::Left:
						TargetCharacter->PlayHitReact(HitDirectionLeftTag, SourceCharacter);
						break;
					case ERPGHitReactDirection::Front:
						TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
						break;
					case ERPGHitReactDirection::Right:
						TargetCharacter->PlayHitReact(HitDirectionRightTag, SourceCharacter);
						break;
					case ERPGHitReactDirection::Back:
						TargetCharacter->PlayHitReact(HitDirectionBackTag, SourceCharacter);
						break;
					}
				}
				else
				{
					// No hit result. Default to front.
					TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
				}

				// Show Damage number for the Source player unless it was self Damage
				if (SourceActor != TargetActor)
				{
					ATopDownController* PC = Cast<ATopDownController>(SourceController);
					if (PC)
					{
						PC->ShowDamageNumber(LocalDamageDone, TargetCharacter);
					}
				}

				if (!TargetCharacter->IsAlive())
				{
					// TargetCharacter was alive before this Damage and now is not alive, give EXP and Gold bounties to Source.
					// Don't give bounty to self.
					if (SourceController != TargetController)
					{
						// Create a dynamic instant Gameplay Effect to give the bounties
						UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
						GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;

						int32 Idx = GEBounty->Modifiers.Num();
						GEBounty->Modifiers.SetNum(Idx + 2);

						FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
						InfoXP.ModifierMagnitude = FScalableFloat(GetEXPBounty());
						InfoXP.ModifierOp = EGameplayModOp::Additive;
						InfoXP.Attribute = URPGAttributeSet::GetEXPAttribute();

						FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
						InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
						InfoGold.ModifierOp = EGameplayModOp::Additive;
						InfoGold.Attribute = URPGAttributeSet::GetGoldAttribute();

						Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
					}
				}
			}
		}
	}// Damage
	else if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		// Handle other HP changes.
		// HP loss should go through Damage.
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
	} // HP
	else if (Data.EvaluatedData.Attribute == GetMPAttribute())
	{
		// Handle MP changes.
		SetMP(FMath::Clamp(GetMP(), 0.0f, GetMaxMP()));
	} // MP
	else if (Data.EvaluatedData.Attribute == GetAPAttribute())
	{
		// Handle AP changes.
		SetAP(FMath::Clamp(GetAP(), 0.0f, GetMaxAP()));
	}
}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, HP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxHP, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, HPRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxMP, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MPRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, AP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MaxAP, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, APRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Damage, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Strength, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Deffense, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Strength, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Agility, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Intelligence, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Luck, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, MoveSpeed, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, CharacterLevel, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, EXP, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, EXPBounty, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, Gold, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSet, GoldBounty, COND_None, REPNOTIFY_OnChanged);
}

void URPGAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void URPGAttributeSet::OnRep_HP(const FGameplayAttributeData& OldHP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, HP, OldHP);
}
void URPGAttributeSet::OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHP, OldMaxHP);
}

void URPGAttributeSet::OnRep_HPRegenRate(const FGameplayAttributeData& OldHPRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, HPRegenRate, OldHPRegenRate);
}

void URPGAttributeSet::OnRep_MP(const FGameplayAttributeData& OldMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MP, OldMP);
}

void URPGAttributeSet::OnRep_MaxMP(const FGameplayAttributeData& OldMaxMP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxMP, OldMaxMP);
}

void URPGAttributeSet::OnRep_MPRegenRate(const FGameplayAttributeData& OldMPRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MPRegenRate, OldMPRegenRate);
}

void URPGAttributeSet::OnRep_AP(const FGameplayAttributeData& OldAP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, AP, OldAP);
}

void URPGAttributeSet::OnRep_MaxAP(const FGameplayAttributeData& OldMaxAP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxAP, OldMaxAP);
}

void URPGAttributeSet::OnRep_APRegenRate(const FGameplayAttributeData& OldAPRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, APRegenRate, OldAPRegenRate);
}

void URPGAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Damage, OldDamage);
}

void URPGAttributeSet::OnRep_Deffense(const FGameplayAttributeData& OldDeffense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Deffense, OldDeffense);
}

void URPGAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Deffense, OldStrength);
}

void URPGAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Deffense, OldAgility);
}

void URPGAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Deffense, OldIntelligence);
}

void URPGAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Deffense, OldLuck);
}

void URPGAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MoveSpeed, OldMoveSpeed);
}

void URPGAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, CharacterLevel, OldCharacterLevel);
}

void URPGAttributeSet::OnRep_EXP(const FGameplayAttributeData& OldXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, EXP, OldXP);
}

void URPGAttributeSet::OnRep_EXPBounty(const FGameplayAttributeData& OldXPBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, EXPBounty, OldXPBounty);
}

void URPGAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Gold, OldGold);
}

void URPGAttributeSet::OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, GoldBounty, OldGoldBounty);
}