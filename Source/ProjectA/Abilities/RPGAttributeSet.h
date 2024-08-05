// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTA_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URPGAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "HP", ReplicatedUsing = OnRep_HP)
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, HP)

		// MaxHP is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "HP", ReplicatedUsing = OnRep_MaxHP)
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHP)

		// HP regen rate will passively increase HP every second
	UPROPERTY(BlueprintReadOnly, Category = "HP", ReplicatedUsing = OnRep_HPRegenRate)
	FGameplayAttributeData HPRegenRate;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, HPRegenRate)

		// Current MP, used to execute special abilities. Capped by MaxMP.
	UPROPERTY(BlueprintReadOnly, Category = "MP", ReplicatedUsing = OnRep_MP)
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MP)

		// MaxMP is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "MP", ReplicatedUsing = OnRep_MaxMP)
	FGameplayAttributeData MaxMP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxMP)

		// MP regen rate will passively increase MP every second
	UPROPERTY(BlueprintReadOnly, Category = "MP", ReplicatedUsing = OnRep_MPRegenRate)
	FGameplayAttributeData MPRegenRate;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MPRegenRate)

		// Current AP, used to execute special abilities. Capped by MaxAP.
	UPROPERTY(BlueprintReadOnly, Category = "AP", ReplicatedUsing = OnRep_AP)
	FGameplayAttributeData AP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, AP)

		// MaxAP is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "AP", ReplicatedUsing = OnRep_MaxAP)
	FGameplayAttributeData MaxAP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxAP)

	// AP regen rate will passively increase AP every second
	UPROPERTY(BlueprintReadOnly, Category = "AP", ReplicatedUsing = OnRep_APRegenRate)
	FGameplayAttributeData APRegenRate;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, APRegenRate)

	// Damage is a meta attribute used by the DamageExecution to calculate final Damage, which then turns into -HP
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Damage)

	// Def reduces the amount of Damage done by attackers
	UPROPERTY(BlueprintReadOnly, Category = "Deffense", ReplicatedUsing = OnRep_Deffense)
	FGameplayAttributeData Deffense;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Deffense)
	
	UPROPERTY(BlueprintReadOnly, Category = "Strength", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Agility", ReplicatedUsing = OnRep_Agility)
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Agility)

	UPROPERTY(BlueprintReadOnly, Category = "Intelligence", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly, Category = "Luck", ReplicatedUsing = OnRep_Luck)
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Luck)

	// MoveSpeed affects how fast characters can move.
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Character Level", ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, CharacterLevel)

	// EXP points gained from killing enemies. Used to level up (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "EXP", ReplicatedUsing = OnRep_EXP)
	FGameplayAttributeData EXP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, EXP)

	// EXP points awarded to the character's killers. Used to level up (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "EXP", ReplicatedUsing = OnRep_EXPBounty)
	FGameplayAttributeData EXPBounty;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, EXPBounty)

	// Gold gained from killing enemies. Used to purchase items (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Gold)

	// Gold awarded to the character's killer. Used to purchase items (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_GoldBounty)
	FGameplayAttributeData GoldBounty;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, GoldBounty)

protected:
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	UFUNCTION()
		virtual void OnRep_HP(const FGameplayAttributeData& OldHP);

	UFUNCTION()
		virtual void OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP);

	UFUNCTION()
		virtual void OnRep_HPRegenRate(const FGameplayAttributeData& OldHPRegenRate);

	UFUNCTION()
		virtual void OnRep_MP(const FGameplayAttributeData& OldMP);

	UFUNCTION()
		virtual void OnRep_MaxMP(const FGameplayAttributeData& OldMaxMP);

	UFUNCTION()
		virtual void OnRep_MPRegenRate(const FGameplayAttributeData& OldMPRegenRate);

	UFUNCTION()
		virtual void OnRep_AP(const FGameplayAttributeData& OldAP);

	UFUNCTION()
		virtual void OnRep_MaxAP(const FGameplayAttributeData& OldMaxAP);

	UFUNCTION()
		virtual void OnRep_APRegenRate(const FGameplayAttributeData& OldAPRegenRate);

	UFUNCTION()
		virtual void OnRep_Damage(const FGameplayAttributeData& OldDamage);
	UFUNCTION()
		virtual void OnRep_Deffense(const FGameplayAttributeData& OldDeffense);

	UFUNCTION()
		virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UFUNCTION()
		virtual void OnRep_Agility(const FGameplayAttributeData& OldAgility);

	UFUNCTION()
		virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);

	UFUNCTION()
		virtual void OnRep_Luck(const FGameplayAttributeData& OldLuck);

	UFUNCTION()
		virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

	UFUNCTION()
		virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);

	UFUNCTION()
		virtual void OnRep_EXP(const FGameplayAttributeData& OldXP);

	UFUNCTION()
		virtual void OnRep_EXPBounty(const FGameplayAttributeData& OldXPBounty);

	UFUNCTION()
		virtual void OnRep_Gold(const FGameplayAttributeData& OldGold);

	UFUNCTION()
		virtual void OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty);

private:
	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
};
