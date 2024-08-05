/*GameplayAbility.h ���� ����Ҽ��ִ��Լ�
* SendGampleEvent - �Ϲ����� �����÷��� �̺�Ʈ�� �߻������� GameplayAbility�� �˸�
* CancelAbility - �ɷ��� ����� ���߿� ���߰� ��� ���� ���·� ����� �Լ�
* UGamplayAbility Ŭ���� ������ �� �Ʒ����� Ư�� GameplayTag�� �߰��ϰų� ������ �� ����� Ȱ��ȭ�ϰų� ����ϴ� ���� UPROPERTY �����ڰ� ���� �ִ�.
* �� �� API���� (https://api.unrealengine.com/Intelligence/API/Plugins/GameplayAbilities/index.html)
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayAbilitySet.h"
#include "AbilitySystemInterface.h"
#include "GameplayTask.h"
#include "ProjectA.h"
#include "C_Common.generated.h"

#define FS(x,...) FString::Printf(TEXT(x), __VA_ARGS__)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, ERPGHitReactDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AC_Common*, Character);

class UCharacterDataAsset;
class URPGGameplayAbility;
class UEquipItem;

UCLASS()
class PROJECTA_API AC_Common : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEquipItem* MainWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEquipItem* SubWeapon;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterDataAsset* CharacterData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Target;

	TWeakObjectPtr<class URPGAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class URPGAttributeSet> AttributeSet;

	// �⺻ ���. ����� ����, ��Ȱ�� �����
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<class URPGGameplayAbility>> CharacterAbilities;

	// ������ �ʱ�ȭ�� �Ӽ�
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// ���ۿ� �Ӽ�
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	

	// ������ �ɷ� �ο�. �ɷ��� ���� Ŭ�� ����
	virtual void AddCharacterAbilities();

	// ���� �� Ŭ�󿡼��� ����
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void SetHP(float HP);
	virtual void SetMP(float MP);
	virtual void SetAP(float AP);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* InteractCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isAI;

public:
	AC_Common(const class FObjectInitializer& ObjectInitializer);

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InputAbilitySystemInputTagPressed(FGameplayTag InputTag);
	void InputAbilitySystemInputTagReleased(FGameplayTag InputTag);
	
	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable, Category = "RPGCharacter")
	virtual int32 GetAbilityLevel(ERPGAbilityInputID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	// Set the Hit React direction in the Animation Blueprint
	UPROPERTY(BlueprintAssignable, Category = "RPGCharacter")
	FCharacterBaseHitReactDelegate ShowHitReact;

	UPROPERTY(BlueprintAssignable, Category = "RPGCharacter")
	FCharacterDiedDelegate OnCharacterDied;
	
	UFUNCTION(BlueprintCallable)
	ERPGHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);
	virtual void PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser);
	virtual bool PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter")
	virtual void FinishDying();

	void Parried(float Duration);
	void Stun(float Duration);
	void Damaged();
	void Finishable();
	void ExecuteFinisherVictim(ACharacter* _Target, int FinisherIndex);
	void SetMoveSpeed();
	virtual void ShotArrow();

	void SetTarget(AActor* _Target) { Target = _Target; }
	

	AActor* GetTarget() { return Target; }
	UCharacterDataAsset* GetCharacterData() { return CharacterData; }
	UEquipItem* GetMainWeapon() { return MainWeapon; }
	UEquipItem* GetSubWeapon() { return SubWeapon; }
	
	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetHP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetMaxHP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetMP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetMaxMP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetAP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetMaxAP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "RPGCharacter|Attributes")
		float GetMoveSpeedBaseValue() const;
};
