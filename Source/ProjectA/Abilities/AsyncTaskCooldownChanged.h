// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AsyncTaskCooldownChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChanged, FGameplayTag, CooldownTag, float, TimeRemaining, float, Duration);
/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class PROJECTA_API UAsyncTaskCooldownChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChanged OnCooldownBegin;

	UPROPERTY(BlueprintAssignable)
	FOnCooldownChanged OnCooldownEnd;

	//UseServerCooldown은 로컬 예측 외 서버의 쿨다운이 반환되는지 여부결정
	//ServerCooldown을 사용하는 경우 TimeRemaining, Duration은 로컬 예측 쿨다운이 시작되었음을 알리기 위해 -1 반환
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncTaskCooldownChanged* ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer CooldownTags, bool UseServerCooldown);

	//종료시 수동으로 호출, UMG 위젯의 경우 Destruct 이벤트에서 호출
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTagContainer CooldownTags;

	bool UseServerCooldown;

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void CooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount);

	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);
};
