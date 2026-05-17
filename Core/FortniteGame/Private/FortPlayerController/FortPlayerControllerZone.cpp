#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

void AFortPlayerControllerZone::ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P) {
	AFortPlayerController::ServerAcknowledgePossessionOG(This, P);
}

void AFortPlayerControllerZone::OnReadyToStartMatch(AFortPlayerControllerZone* This) {
	OnReadyToStartMatchOG(This);

	AFortGameModeZone* GameMode = This->GetWorld()->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (GameMode) {
		if (GameMode->StartingItems.Num() > 0)
		{
			Log("OnReadyToStartMatch: Processing StartingItems for new player. Count: " + std::to_string(GameMode->StartingItems.Num()));
			for (int i = 0; i < GameMode->StartingItems.Num(); i++)
			{
				auto& StartingItem = GameMode->StartingItems.GetWithSize(i, FItemAndCount::GetSize());

				Log(std::format("StartingItem {}: Item={}, Count={}", i, StartingItem.Item ? StartingItem.Item->GetFName().ToString().ToString() : "None", StartingItem.Count));
				if (StartingItem.Count)
					This->WorldInventory->AddItem(StartingItem.Item, StartingItem.Count);
			}
		}
		else {
			Log(" Warning: No StartingItems found!");

			static UFortItemDefinition* DefaultPickaxe = nullptr;
			if (!DefaultPickaxe && GameMode->IsA(AFortGameModeAthena::StaticClass())) {
				DefaultPickaxe = (UFortItemDefinition*)StaticLoadObject("/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
			}
			else {
				DefaultPickaxe = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/Melee/Harvest/WID_Harvest_Pickaxe_C_T01.WID_Harvest_Pickaxe_C_T01");
			}

			static UFortItemDefinition* WallBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
			static UFortItemDefinition* FloorBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
			static UFortItemDefinition* StairBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W");
			static UFortItemDefinition* ConeBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS");
			static UFortItemDefinition* EditTool = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");

			if (DefaultPickaxe) {
				This->WorldInventory->AddItem(DefaultPickaxe);
			}
			if (WallBuild) {
				This->WorldInventory->AddItem(WallBuild);
			}
			if (FloorBuild) {
				This->WorldInventory->AddItem(FloorBuild);
			}
			if (StairBuild) {
				This->WorldInventory->AddItem(StairBuild);
			}
			if (ConeBuild) {
				This->WorldInventory->AddItem(ConeBuild);
			}
			if (EditTool) {
				This->WorldInventory->AddItem(EditTool);
			}
		}
	}
}

void AFortPlayerControllerZone::ServerSendLoadoutConfig(AFortPlayerControllerZone* This, int32 LoadoutSeed, TArray<uint8>& Loadout) {
	Log("ServerSendLoadoutConfig Called!");
	Log("LoadoutSeed: " + std::to_string(LoadoutSeed));
	Log("Loadout count: " + std::to_string(Loadout.Num()));

	ServerSendLoadoutConfigOG(This, LoadoutSeed, Loadout);
}

void AFortPlayerControllerZone::ServerSetShouldDisablePlayerTeleportingDuringMissionResults(AFortPlayerControllerZone* This) {
	ServerSetShouldDisablePlayerTeleportingDuringMissionResultsOG(This);

	Log("ServerSetShouldDisablePlayerTeleportingDuringMissionResults called!");
}

void AFortPlayerControllerZone::Hook() {
	HookEveryVTable(AFortPlayerControllerZone::StaticClass(), AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), ServerAcknowledgePossession, nullptr);
	
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerControllerZone_OnReadyToStartMatch()), OnReadyToStartMatch, (LPVOID*)&OnReadyToStartMatchOG);

	HookEveryVTable(
		AFortPlayerControllerZone::StaticClass(),
		AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerControllerZone.ServerSendLoadoutConfig"),
		ServerSendLoadoutConfig,
		(LPVOID*)&ServerSendLoadoutConfigOG
	);

	HookEveryVTable(
		AFortPlayerControllerZone::StaticClass(),
		AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerControllerZone.ServerSetShouldDisablePlayerTeleportingDuringMissionResults"),
		ServerSetShouldDisablePlayerTeleportingDuringMissionResults,
		(LPVOID*)&ServerSetShouldDisablePlayerTeleportingDuringMissionResultsOG
	);

	Log("Hooked AFortPlayerControllerZone");
}