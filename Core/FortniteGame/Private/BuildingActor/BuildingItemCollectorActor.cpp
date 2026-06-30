#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingItemCollectorActor.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerController.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

void ABuildingItemCollectorActor::GrantOutput() {
	Log("ABuildingItemCollectorActor::VendWobble__FinishedFunc: " + GetName().ToString());
	
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingItemCollectorActor::VendWobble__FinishedFunc: World is nullptr!");
		return;
	}
	
	AFortPlayerController* PC = ControllingPlayer;
	if (!PC) {
		Log("ABuildingItemCollectorActor::VendWobble__FinishedFunc: PC is nullptr!");
		return;
	}

	FColletorUnitInfo* Collection = nullptr;
	for (int32 i = 0; i < ItemCollections.Num(); i++) {
		FColletorUnitInfo& CollectorUnitInfo = ItemCollections.GetWithSize(i, FColletorUnitInfo::GetSize());
		if (CollectorUnitInfo.InputItem == ClientPausedActiveInputItem) {
			Collection = &CollectorUnitInfo;
			break;
		}
	}

	if (!Collection) {
		Log("ABuildingItemCollectorActor::VendWobble__FinishedFunc: Collection not found!");
		return;
	}

	ClientPausedActiveInputItem = nullptr;

	float Cost = Collection->InputCount.Evaluate();
	UFortItemDefinition* InputItemDef = Collection->InputItem;

	FFortItemEntry* ItemEntry = PC->WorldInventory->FindItemEntry(InputItemDef);
	if (ItemEntry) {
		PC->WorldInventory->RemoveItem(ItemEntry->ItemGuid, Cost);
	}

	FVector ItemCollectorLocation = K2_GetActorLocation();
	FVector FinalSpawnLocation = ItemCollectorLocation + (GetActorForwardVector() * LootSpawnLocation.X) +
		(GetActorRightVector() * LootSpawnLocation.Y) + (GetActorUpVector() * LootSpawnLocation.Z);

	for (int i = 0; i < Collection->OutputItemEntry.Num(); i++) {
		FFortItemEntry& ItemEntry = Collection->OutputItemEntry.GetWithSize(i, FFortItemEntry::GetSize());
		if (!ItemEntry.ItemDefinition) {
			continue;
		}

		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry.ItemDefinition,
			ItemEntry.Count,
			FinalSpawnLocation,
			FVector(),
			-1,
			true,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetContainer(),
			EFortPickupSpawnSource::GetVendingMachine(),
			nullptr,
			false
		);
		if (Pickup) {
			UFortWeaponItemDefinition* WeaponDef = Pickup->PrimaryPickupItemEntry.ItemDefinition->Cast<UFortWeaponItemDefinition>();
			if (WeaponDef) {
				int32 Level = Pickup->PrimaryPickupItemEntry.Level;
				Pickup->PrimaryPickupItemEntry.LoadedAmmo = WeaponDef->GetClipSize(Level);
				Pickup->PrimaryPickupItemEntry.Durability = WeaponDef->GetDurability(Level);
				Pickup->PrimaryPickupItemEntry.bIsDirty = true;
				Pickup->PrimaryPickupItemEntry.ReplicationKey++;
				Pickup->OnRep_PrimaryPickupItemEntry();
			}
		}
	}

	if (Cost <= 0) {
		K2_DestroyActor();
	}

	// reset it
	ControllingPlayer = nullptr;
}

void ABuildingItemCollectorActor::BeginPlay(ABuildingItemCollectorActor* This) {
	BeginPlayOG(This);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return;
	}

	for (int32 i = 0; i < This->ItemCollections.Num(); i++) {
		FColletorUnitInfo& CollectorUnitInfo = This->ItemCollections.GetWithSize(i, FColletorUnitInfo::GetSize());
		if (CollectorUnitInfo.bUseDefinedOutputItem) {
			continue;
		}

		if (CollectorUnitInfo.OutputItemEntry.Num() > 0) {
			CollectorUnitInfo.OutputItemEntry.Empty();
			CollectorUnitInfo.OutputItem = nullptr;
		}

		TArray<FFortItemEntry> LootDrops;
		bool bSuccess = UFortKismetLibrary::PickLootDrops(World, &LootDrops, This->DefaultItemLootTierGroupName, -1, -1);

		if (bSuccess) {
			for (int32 j = 0; j < LootDrops.Num(); j++) {
				FFortItemEntry& ItemEntry = LootDrops.GetWithSize(j, FFortItemEntry::GetSize());

				if (ItemEntry.ItemDefinition) {
					if (!CollectorUnitInfo.OutputItem) {
						CollectorUnitInfo.OutputItem = ItemEntry.ItemDefinition->Cast<UFortWorldItemDefinition>();
					}

					CollectorUnitInfo.OutputItemEntry.Add(ItemEntry);
				}
			}
		}
	}
}

void ABuildingItemCollectorActor::Hook() {
	HookEveryVTableIdx(
		ABuildingItemCollectorActor::StaticClass(),
		Finder::FindAActor_BeginPlayVFT(),
		BeginPlay,
		(LPVOID*)&BeginPlayOG
	);

	Log("ABuildingItemCollectorActor Hooked");
}