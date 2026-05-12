#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"

void AFortPlayerControllerAthena::EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft) {
	EnterAircraftOG(This, InAircraft);

	if (This->WorldInventory) {
		TArray<FGuid> GuidsToRemove;
		for (int32 i = 0; i < This->WorldInventory->Inventory.ReplicatedEntries.Num(); i++) {
			FFortItemEntry& Entry = This->WorldInventory->Inventory.ReplicatedEntries[i];

			if (UFortWorldItemDefinition* WorldItemDef = Entry.ItemDefinition->Cast<UFortWorldItemDefinition>()) {
				if (WorldItemDef->bCanBeDropped) {
					GuidsToRemove.Add(Entry.ItemGuid);
				}
			}
		}

		for (const FGuid& Guid : GuidsToRemove) {
			This->WorldInventory->RemoveItem(Guid);
		}
	}
}

void AFortPlayerControllerAthena::ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation) {
	ServerAttemptAircraftJumpOG(This, ClientRotation);

	Log("ServerAttemptAircraftJump Called.");

	if (This->WorldInventory) {
		if (This->IsUsingOldQuickBars()) {
			This->WorldInventory->EquipHarvestingTool();
		}
	}
}