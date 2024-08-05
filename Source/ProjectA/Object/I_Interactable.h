// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_Interactable.generated.h"

class AC_Player;

UINTERFACE(MinimalAPI)
class UI_Interactable : public UInterface
{
	GENERATED_BODY()
};
class PROJECTA_API II_Interactable
{
	GENERATED_BODY()	
public:
	virtual void InteractEvent(AC_Player* Player) = 0;
	virtual void SetWidgetVisibility(bool IsVisible) = 0;
	virtual int GetInteractableType() { return 0; }
};
