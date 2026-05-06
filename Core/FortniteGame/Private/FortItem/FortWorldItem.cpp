#include "pch.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"

#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

EFortQuickBars UFortWorldItem::GetQuickBarForItem() {
	if (!ItemEntry.ItemDefinition) {
		Log("UFortWorldItem::GetQuickBarForItem: ItemEntry.ItemDefinition is null!");
		return EFortQuickBars::Max_None;
	}

	return ItemEntry.ItemDefinition->GetQuickBarForItem();
}