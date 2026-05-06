#pragma once
#include "pch.h"

#include "BuildingAutoNav.h"

class AFortPlayerPawn;

class ABuildingContainer : public ABuildingAutoNav {
public:
	DefineUnrealClass(ABuildingContainer);

public:
	static bool SpawnLoot(ABuildingContainer* This, UObject* Object);

	static void Hook() {
		HookEveryVTableIdx(
			ABuildingContainer::StaticClass(),
			Finder::FindABuildingContainer_SpawnLootVFT(),
			SpawnLoot
		);

		Log("ABuildingContainer Hooked!");
	}
};