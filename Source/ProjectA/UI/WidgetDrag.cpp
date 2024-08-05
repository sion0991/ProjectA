// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDrag.h"

void UWidgetDrag::Init(const TObjectPtr<UWidget>& _DragVisual, EDragPivot _Pivot, FVector2D _Offset, const TObjectPtr<UUserWidget>& _WidgetRef, FVector2D _DragOffset)
{
	DefaultDragVisual = _DragVisual;
	Pivot = _Pivot;
	Offset = _Offset;
	WidgetReference = _WidgetRef;
	DragOffset = _DragOffset;
}
