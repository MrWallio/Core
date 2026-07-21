#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BGAConsumableSpawner.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"

bool ABGAConsumableSpawner::AttemptSpawn() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return false;
	}

	AFortGameStateZone* GameState = World->GameState->Cast<AFortGameStateZone>();
	if (!GameState) {
		return false;
	}

	TArray<FFortItemEntry> LootDrops;
	if (!SpawnLootTierGroup.IsNone()) {
		UFortKismetLibrary::PickLootDrops(World, &LootDrops, SpawnLootTierGroup, GameState->WorldLevel, -1);
	}

	TArray<FFortItemEntry>& Consumables = LootDrops.Num() > 0 ? LootDrops : ConsumablesToSpawn;

	bool bSpawnedAny = false;

	for (int32 i = 0; i < Consumables.Num(); i++) {
		FFortItemEntry& ItemEntry = Consumables.GetWithSize(i, FFortItemEntry::GetSize());
		if (!ItemEntry.ItemDefinition) {
			continue;
		}

		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry.ItemDefinition,
			ItemEntry.Count,
			K2_GetActorLocation(),
			*FVector::Allocate(),
			-1,
			bAlignSpawnedActorsToSurface,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetOther(),
			EFortPickupSpawnSource::GetItemSpawner(),
			nullptr,
			false
		);

		if (Pickup) {
			bSpawnedAny = true;
		}
	}

	return bSpawnedAny;
}