// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InventorySlot.generated.h"

class UImage;
class UUW_InventorySlot;

DECLARE_MULTICAST_DELEGATE_TwoParams(FDropAtSlot, UUW_InventorySlot*, UUW_InventorySlot*);
DECLARE_MULTICAST_DELEGATE_OneParam(FDropItem, UUW_InventorySlot*);

UCLASS()
class PROJECTA_API UUW_InventorySlot : public UUserWidget
{
	GENERATED_BODY()
private:
	void Init(const UUW_InventorySlot& InventorySlot);
public:
	
	FDropAtSlot DropAtSlot;
	FDropItem DropItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Background;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* PickUpImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SlotIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D DragOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SlotType;

	FSlateBrush MakeBrushFromTexture(UTexture2D* Texture, int32 Width = 0, int32 Height = 0);
	FSlateBrush GetInventoryImage();


	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
