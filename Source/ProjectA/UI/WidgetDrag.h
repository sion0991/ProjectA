// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDrag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UWidgetDrag : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	void Init(const TObjectPtr<UWidget>& _DragVisual, EDragPivot _Pivot, FVector2D _Offset, const TObjectPtr<UUserWidget>& _WidgetRef, FVector2D _DragOffset);
	class UUserWidget* WidgetReference;
	FVector2D DragOffset;
};
