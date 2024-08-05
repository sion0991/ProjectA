// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_ItemText.h"
#include "Components/TextBlock.h"

void UUW_ItemText::SetText(FText Text)
{
	TB_PickUp->SetText(Text);
}
