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