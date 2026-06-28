#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortWorld/FortWorldManager.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"
#include "FortniteGame/Public/FortPlayer/FortPlayerDeathReport.h"
#include "FortniteGame/Public/Info/FortTeamInfo.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistAthena.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistManager.h"

bool AFortGameModeAthena::ReadyToStartMatch(AFortGameModeAthena* This) {
	if (This->bWorldIsReady
		&& This->MatchState == MatchState::WaitingToStart
		&& This->NumPlayers >= This->WarmupRequiredPlayerCount) {

		Log(std::format("ReadyToStartMatch: Match ready to start! Players={}/{}, WorldReady={}",
			This->NumPlayers,
			This->WarmupRequiredPlayerCount,
			This->bWorldIsReady));
		return true;
	}

	return false;
}

APawn* AFortGameModeAthena::SpawnDefaultPawnFor(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot) {
	APawn* Pawn = AFortGameModeZone::SpawnDefaultPawnFor(This, NewPlayer, StartSpot);
	if (!Pawn) {
		// sometimes it doesent work when you get kicked from the bus because the startspot is obstructed by another pawn
		Pawn = This->SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetTransform());
	}

	Log("AFortGameModeAthena::SpawnDefaultPawnFor: Spawned default pawn. NewPlayer=" + (NewPlayer ? NewPlayer->GetName().ToString() : "None") + " Pawn=" + (Pawn ? Pawn->GetName().ToString() : "None"));
	return Pawn;
}

void AFortGameModeAthena::FinishWorldInitialization(AFortGameModeAthena* This, AFortWorldManager* WorldManager) {
	AFortGameModeZone::FinishWorldInitialization(This, WorldManager);
	FinishWorldInitializationOG(This, WorldManager);
	
	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("FinishWorldInitialization: GameState is null or not AFortGameStateAthena");
		return;
	}

	This->DefaultPawnClass = (UClass*)StaticLoadObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
	//This->PlayerControllerClass = (UClass*)StaticLoadObject("/Game/Athena/Athena_PlayerController.Athena_PlayerController_C");

	This->bDisableGCOnServerDuringMatch = true;

	GameState->OnRep_CurrentPlaylistData();
	GameState->OnRep_CurrentPlaylistInfo();
}

void AFortGameModeAthena::AddToAlivePlayers(AFortPlayerControllerAthena* PC) {
	void (*AddToAlivePlayersInternal)(AFortGameModeAthena* This, AFortPlayerControllerAthena* PC) = decltype(AddToAlivePlayersInternal)(ImageBase + Finder::FindAFortGameModeAthena_AddToAlivePlayers());
	AddToAlivePlayersInternal(this, PC);
}

void AFortGameModeAthena::RemoveFromAlivePlayers(AFortPlayerControllerAthena* PC, APlayerState* RemovalInstigator, APawn* FinisherPawn, UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause, bool bIsTeamSwitching) {
	void (*RemoveFromAlivePlayersInternal)(AFortGameModeAthena * This, AFortPlayerControllerAthena * PC, APlayerState * RemovalInstigator, APawn * FinisherPawn, UFortWeaponItemDefinition * FinishingWeapon, uint8 DeathCause, bool bIsTeamSwitching) = decltype(RemoveFromAlivePlayersInternal)(ImageBase + Finder::FindAFortGameModeAthena_RemoveFromAlivePlayers());
	RemoveFromAlivePlayersInternal(this, PC, RemovalInstigator, FinisherPawn, FinishingWeapon, DeathCause, bIsTeamSwitching);
}

int32 AFortGameModeAthena::StartAircraftPhase(AFortGameModeAthena* This, bool bGoStraightToSafeZone) {
	for (AFortPlayerControllerAthena* PC : This->AlivePlayers) {
		if (PC->WorldInventory) {
			PC->WorldInventory->DropAllItems(false);
		}

		if (PC->MyFortPawn || PC->Pawn->Cast<AFortPlayerPawn>()) {
			AFortPlayerPawn* MyFortPawn = PC->MyFortPawn ? PC->MyFortPawn : PC->Pawn->Cast<AFortPlayerPawn>();

			MyFortPawn->SetHealth(MyFortPawn->GetMaxHealth());
			MyFortPawn->SetShield(0.0f);
		}
	}

	return StartAircraftPhaseOG(This, bGoStraightToSafeZone);
}

uint8 AFortGameModeAthena::PickTeam(AFortGameModeAthena* This, uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor) {
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bDevSameTeam) {
		return 0;
	}
	if (Config.bUseGameSessions) {
		return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
	}

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::PickTeam: GameState is null or not AFortGameStateAthena");
		return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
	}

	for (AFortTeamInfo* TeamInfo : GameState->Teams) {
		if (!TeamInfo) {
			continue;
		}

		int32 TeamMemberCount = TeamInfo->TeamMembers.Num();
		if (TeamMemberCount < GameState->TeamSize) {
			Log("AFortGameModeAthena::PickTeam: Assigning player to team " + std::to_string(TeamInfo->Team) + " with " + std::to_string(TeamMemberCount) + " members.");
			return TeamInfo->Team;
		}
	}

	return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
}

void AFortGameModeAthena::InitGameState(AFortGameModeAthena* This) {
	InitGameStateOG(This);
	
	FCoreConfig& Config = ConfigurationManager::GetConfig();

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::InitGameState: GameState is null or not AFortGameStateAthena");
		return;
	}

	GameState->SetCurrentPlaylistId(This->CurrentPlaylistId);
	if (UFortPlaylistManager::StaticClass()) {
		UFortPlaylistManager* PlaylistManager = UFortPlaylistManager::Get();
		UFortPlaylistAthena* Playlist = nullptr;
		if (PlaylistManager) {
			// We need to check if Config.Playlist is a number or a string and thats how we will find the playlist
			if (Config.Playlist.find_first_not_of("0123456789") == std::string::npos) {
				int32 PlaylistId = std::stoi(Config.Playlist);
				Playlist = PlaylistManager->GetPlaylist(PlaylistId);
			}
			else {
				FName PlaylistName = UKismetStringLibrary::Conv_StringToName(Config.Playlist);
				Playlist = PlaylistManager->GetPlaylist(PlaylistName);
			}
		}
		else {
			Log("AFortGameModeAthena::InitGameState: Failed to get PlaylistManager");
		}

		if (Playlist) {
			if (GameState->_HasCurrentPlaylistData()) {
				GameState->CurrentPlaylistData = Playlist;
			}
			if (GameState->_HasCurrentPlaylistInfo()) {
				GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
				GameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
				GameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
				GameState->CurrentPlaylistInfo.MarkArrayDirty();
			}

			This->GameSession->MaxPlayers = Playlist->MaxPlayers;
			This->GameSession->MaxPartySize = Playlist->MaxSquadSize;

			Log("AFortGameModeAthena::InitGameState: Applied playlist " + Playlist->GetFName().ToString().ToString());
		}
		else {
			Log("AFortGameModeAthena::InitGameState: Failed to get Playlist");
		}
	}
	else {
		This->GameSession->MaxPlayers = 100;
		This->MaxPlayerCount = 100;
	}
}