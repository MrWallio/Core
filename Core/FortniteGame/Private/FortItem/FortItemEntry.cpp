#include "pch.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"

#include "FortniteGame/Public/FortItem/FortItemEntryStateValue.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

void FFortItemEntry::SetStateValue(EFortItemEntryState StateType, int32 InValue) {
	for (FFortItemEntryStateValue& StateValue : StateValues) {
		if (StateValue.StateType == StateType) {
			StateValue.IntValue = InValue;
			return;
		}
	}

	FFortItemEntryStateValue StateValue = FFortItemEntryStateValue(StateType);
	StateValue.IntValue = InValue;
	StateValues.Add(StateValue);
}

void FFortItemEntry::SetToDirty() {
	if (ParentInventory.Get()) {
		ParentInventory.Get()->Update(this);
	}
	bIsDirty = true;
}