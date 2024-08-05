#pragma once

#include "CoreMinimal.h"
#include "Inventory.generated.h"

struct FST_Item;

USTRUCT(Blueprintable, BlueprintType)
struct FInventory
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Inventory)
	TArray<FST_Item> EquipItems;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Inventory)
	TArray<FST_Item> InventoryItems;

	FInventory operator=(const FInventory& Inventory)
	{
		this->EquipItems = Inventory.EquipItems;
		this->InventoryItems = Inventory.InventoryItems;
		return *this;
	}
};
