// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ST_Stat.generated.h"

USTRUCT(BlueprintType)
struct FStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float HP = 0;
	UPROPERTY(EditAnywhere)
		float MP = 0;
	UPROPERTY(EditAnywhere)
		float AP = 0;
	UPROPERTY(EditAnywhere)
		int Attack = 0;
	UPROPERTY(EditAnywhere)
		int Defense = 0;
	UPROPERTY(EditAnywhere)
		int Strangth = 0;
	UPROPERTY(EditAnywhere)
		int Dexterity = 0;
	UPROPERTY(EditAnywhere)
		int Intelligence = 0;
	UPROPERTY(EditAnywhere)
		int Luck = 0;

	void Reset()
	{
		HP = 0;
		MP = 0;
		AP = 0;
		Attack = 0;
		Defense = 0;
		Strangth = 0;
		Dexterity = 0;
		Intelligence = 0;
		Luck = 0;
	}
	void operator+=(const FStat& ref)
	{
		HP += ref.HP;
		MP += ref.MP;
		AP += ref.AP;
		Attack += ref.Attack;
		Defense += ref.Defense;
		Strangth += ref.Strangth;
		Dexterity += ref.Dexterity;
		Intelligence += ref.Intelligence;
		Luck += ref.Luck;
	}
	FStat operator+(const FStat& ref)
	{
		FStat Result = FStat();
		//Result.Reset();
		Result.HP = this->HP + ref.HP;
		Result.MP = this->MP + ref.MP;
		Result.AP = this->AP + ref.AP;
		Result.Attack = this->Attack + ref.Attack;
		Result.Defense = this->Defense + ref.Defense;
		Result.Strangth = this->Strangth + ref.Strangth;
		Result.Dexterity = this->Dexterity + ref.Dexterity;
		Result.Intelligence = this->Intelligence + ref.Intelligence;
		Result.Luck = this->Luck + ref.Luck;
		return Result;
	}
};