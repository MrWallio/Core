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
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

void AFortPlayerControllerZone::ServerAcknowledgePossession(AFortPlayerPawnAthena* P) {
	AFortPlayerController::ServerAcknowledgePossessionOG(this, P);
}

void AFortPlayerControllerZone::ServerAcknowledgePossessionHK(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P) {
	if (!This || !P) return ServerAcknowledgePossessionOG(This, P);

	This->ServerAcknowledgePossession(P);
}

void AFortPlayerControllerZone::OnReadyToStartMatch() {
	OnReadyToStartMatchOG(this);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerControllerZone::OnReadyToStartMatch: World is null!");
		return;
	}

	AFortGameModeZone* GameMode = GetWorld()->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!GameMode) {
		Log("AFortPlayerControllerZone::OnReadyToStartMatch: GameMode is null or not a FortGameModeZone!");
		return;
	}

	if (GameMode) {
		if (GameMode->StartingItems.Num() > 0)
		{
			Log("OnReadyToStartMatch: Processing StartingItems for new player. Count: " + std::to_string(GameMode->StartingItems.Num()));
			for (int i = 0; i < GameMode->StartingItems.Num(); i++)
			{
				auto& StartingItem = GameMode->StartingItems.GetWithSize(i, FItemAndCount::GetSize());

				Log(std::format("StartingItem {}: Item={}, Count={}", i, StartingItem.Item ? StartingItem.Item->GetFName().ToString().ToString() : "None", StartingItem.Count));
				if (StartingItem.Count)
					WorldInventory->AddItem(StartingItem.Item, StartingItem.Count);
			}
		}
		else {
			Log(" Warning: No StartingItems found!");

			static UFortItemDefinition* DefaultPickaxe = DefaultPickaxe = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/Melee/Harvest/WID_Harvest_Pickaxe_C_T01.WID_Harvest_Pickaxe_C_T01");

			static UFortItemDefinition* WallBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
			static UFortItemDefinition* FloorBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
			static UFortItemDefinition* StairBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W");
			static UFortItemDefinition* ConeBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS");
			static UFortItemDefinition* EditTool = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");

			if (DefaultPickaxe) {
				WorldInventory->AddItem(DefaultPickaxe);
			}
			if (WallBuild) {
				WorldInventory->AddItem(WallBuild);
			}
			if (FloorBuild) {
				WorldInventory->AddItem(FloorBuild);
			}
			if (StairBuild) {
				WorldInventory->AddItem(StairBuild);
			}
			if (ConeBuild) {
				WorldInventory->AddItem(ConeBuild);
			}
			if (EditTool) {
				WorldInventory->AddItem(EditTool);
			}
		}
	}
}

void AFortPlayerControllerZone::OnReadyToStartMatchHK(AFortPlayerControllerZone* This) {
	if (!This) return OnReadyToStartMatchOG(This);

	This->OnReadyToStartMatch();
}

void AFortPlayerControllerZone::ServerReturnToMainMenu() {
	ServerReturnToMainMenuOG(this);
}

void AFortPlayerControllerZone::ServerReturnToMainMenuHK(AFortPlayerControllerZone* This) {
	if (!This) return ServerReturnToMainMenuOG(This);

	This->ServerReturnToMainMenu();
}

void AFortPlayerControllerZone::Hook() {
	HookEveryVTable(AFortPlayerControllerZone::StaticClass(), AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), ServerAcknowledgePossessionHK, (LPVOID*)ServerAcknowledgePossessionOG);
	
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerControllerZone_OnReadyToStartMatch()), OnReadyToStartMatchHK, (LPVOID*)&OnReadyToStartMatchOG);

	HookVTable(
		AFortPlayerControllerZone::GetDefaultObj(),
		AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerReturnToMainMenu"),
		ServerReturnToMainMenuHK,
		(LPVOID*)&ServerReturnToMainMenuOG
	);

	Log("Hooked AFortPlayerControllerZone");
}