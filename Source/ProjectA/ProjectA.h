// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"



#define ACTOR_ROLE_FSTRING *(FindObject<UEnum>(nullptr, TEXT("/Script/Engine.ENetRole"), true)->GetNameStringByValue(GetLocalRole()))
#define GET_ACTOR_ROLE_FSTRING(Actor) *(FindObject<UEnum>(nullptr, TEXT("/Script/Engine.ENetRole"), true)->GetNameStringByValue(Actor->GetLocalRole()))

UENUM(BlueprintType)
enum class ERPGHitReactDirection : uint8
{
	// 0
	None			UMETA(DisplayName = "None"),
	// 1
	Left 			UMETA(DisplayName = "Left"),
	// 2
	Front 			UMETA(DisplayName = "Front"),
	// 3
	Right			UMETA(DisplayName = "Right"),
	// 4
	Back			UMETA(DisplayName = "Back")
};

UENUM(BlueprintType)
enum class ERPGAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "IA_None"),
	// 1 Confirm
	IA_Confirm			UMETA(DisplayName = "IA_Confirm"),
	// 2 Cancel
	IA_Cancel			UMETA(DisplayName = "IA_Cancel"),
	// 3 
	IA_Ability1		UMETA(DisplayName = "IA_Ability1"),
	// 4 
	IA_Ability2		UMETA(DisplayName = "IA_Ability2"),
	// 5 
	IA_Ability3		UMETA(DisplayName = "IA_Ability3"),
	// 6 
	IA_Ability4		UMETA(DisplayName = "IA_Ability4"),
	// 7 
	IA_AbilityQ		UMETA(DisplayName = "IA_AbilityQ"),
	// 7 
	IA_AbilityE		UMETA(DisplayName = "IA_AbilityE"),
	// 7 
	IA_AbilityR		UMETA(DisplayName = "IA_AbilityR"),
	// 7 
	IA_AbilityMUp		UMETA(DisplayName = "IA_AbilityMUp"),
	// 7 
	IA_AbilityMDown	UMETA(DisplayName = "IA_AbilityMDown"),
	// 7 
	IA_AbilityMR		UMETA(DisplayName = "IA_AbilityMR"),
	// 7 
	IA_AbilityML		UMETA(DisplayName = "IA_AbilityML"),
	// 8 Sprint
	IA_Sprint			UMETA(DisplayName = "IA_Sprint"),
	// 9 Jump
	IA_Jump			UMETA(DisplayName = "IA_Jump")
};
