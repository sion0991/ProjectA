// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_InventorySlot.h"
#include "WidgetDrag.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UUW_InventorySlot::Init(const UUW_InventorySlot& InventorySlot)
{
	PickUpImage = InventorySlot.PickUpImage;
	SlotIndex = InventorySlot.SlotIndex;
	SlotType = InventorySlot.SlotType;
}

FSlateBrush UUW_InventorySlot::MakeBrushFromTexture(UTexture2D* Texture, int32 Width, int32 Height)
{
	if (Texture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Texture);
		Width = (Width > 0) ? Width : Texture->GetSizeX();
		Height = (Height > 0) ? Height : Texture->GetSizeY();
		Brush.ImageSize = FVector2D(Width, Height);
		return Brush;
	}
	return FSlateNoResource();
}

FSlateBrush UUW_InventorySlot::GetInventoryImage()
{
	return MakeBrushFromTexture(PickUpImage);
}

FReply UUW_InventorySlot::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if (InMouseEvent.GetEffectingButton() == DragKey /*|| PointerEvent.IsTouchEvent()*/)
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();

		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
			}
		}
		return Reply.NativeReply;
	}
	return FReply::Unhandled();
}

FReply UUW_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//FReply::DetectDrag(InMouseEvent, FKey::FKey("LeftMouseButton"));
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("InventorySlotClick")));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ButtonDownInventorySlot"));
	DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

void UUW_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UUW_InventorySlot* TempSlot = Cast<UUW_InventorySlot>(CreateWidget(GetOwningPlayer(), UUW_InventorySlot::GetClass()));
	TempSlot->Init(*this);
	UWidgetDrag* DragDropOperation = NewObject<UWidgetDrag>(); 
	UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDrag::StaticClass()); //로도 가능
	DragDropOperation->Init(TempSlot, EDragPivot::CenterCenter, FVector2D::ZeroVector, TempSlot, DragOffset);

	OutOperation = DragDropOperation;
}

bool UUW_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InDragDropEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UUserWidget* DragRef = Cast<UWidgetDrag>(InOperation)->WidgetReference;
		UUW_InventorySlot* WidgetDrag = Cast<UUW_InventorySlot>(DragRef);
		if(DropAtSlot.IsBound())
			DropAtSlot.Broadcast(WidgetDrag, this);

		DragRef->RemoveFromParent();
	}
	return false;
}

void UUW_InventorySlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	UWidgetDrag* WidgetDrag = Cast<UWidgetDrag>(InOperation);

	if (WidgetDrag)
	{
		if (DropItem.IsBound())
			DropItem.Broadcast(this);
		WidgetDrag->WidgetReference->RemoveFromParent();
	}
}
