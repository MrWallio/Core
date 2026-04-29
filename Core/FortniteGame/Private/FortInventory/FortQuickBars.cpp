#include "pch.h"
#include "FortniteGame/Public/FortInventory/FortQuickBars.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"

void AFortQuickBars::ServerAddItemInternal(const FGuid& Item, EFortQuickBars InQuickBar, int32 Slot)
{
	if (!this) return;

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerAddItemInternal");

	if (!Func) {
		Log("AFortQuickBars::ServerAddItemInternal: Failed to find function!");
		return;
	}

	struct FortQuickBars_ServerAddItemInternal
	{
	public:
		FGuid Item;
		EFortQuickBars InQuickBar;
		int32 Slot;
	};

	FortQuickBars_ServerAddItemInternal Parms{};

	Parms.Item = std::move(Item);
	Parms.InQuickBar = InQuickBar;
	Parms.Slot = Slot;

	ProcessEvent(Func, &Parms);
}

EFortQuickBars AFortQuickBars::GetQuickBarForItem(UFortItemDefinition* ItemDefinition) {
	if (!ItemDefinition) {
		Log("AFortQuickBars::GetQuickBarForItem: Cannot get quick bar for item, ItemDefinition is invalid!");
		return EFortQuickBars::Max_None;
	}

	if (ItemDefinition->IsA(UFortEditToolItemDefinition::StaticClass())
		|| ItemDefinition->IsA(UFortBuildingItemDefinition::StaticClass())
		|| ItemDefinition->IsA(UFortAmmoItemDefinition::StaticClass())
		|| ItemDefinition->IsA(UFortResourceItemDefinition::StaticClass())
		|| ItemDefinition->IsA(UFortTrapItemDefinition::StaticClass()))
		return EFortQuickBars::Secondary;

	return EFortQuickBars::Primary;
}

EFortQuickBars AFortQuickBars::GetQuickBarForItem(UFortWorldItem* Item) {
	if (!Item) {
		Log("AFortQuickBars::GetQuickBarForItem: Cannot get quick bar for item, Item is invalid!");
		return EFortQuickBars::Max_None;
	}

	return GetQuickBarForItem(Item->ItemEntry.ItemDefinition);
}