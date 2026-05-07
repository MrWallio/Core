#include "pch.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"

#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

uint8 UFortWorldItem::GetQuickBarForItem() {
	if (!ItemEntry.ItemDefinition) {
		Log("UFortWorldItem::GetQuickBarForItem: ItemEntry.ItemDefinition is null!");
		return EFortQuickBars::GetMax_None();
	}

	return ItemEntry.ItemDefinition->GetQuickBarForItem();
}