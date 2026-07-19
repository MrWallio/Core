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
#include "FortniteGame/Public/Athena/FortAthenaMapInfo.h"
#include "FortniteGame/Public/FortSupplyDrop/FortSupplyDropInfo.h"

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

bool AFortGameModeAthena::SetupPlaylist() {
	FCoreConfig& Config = ConfigurationManager::GetConfig();

	AFortGameStateAthena* FortGameState = GameState->Cast<AFortGameStateAthena>();
	if (!FortGameState) {
		Log("AFortGameModeAthena::SetupPlaylist: GameState is null or not AFortGameStateAthena");
		return false;
	}

	UFortPlaylistAthena* Playlist = nullptr;
	if (UFortPlaylistManager::StaticClass()) {
		UFortPlaylistManager* PlaylistManager = UFortPlaylistManager::Get();
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
	}

	int32 MaxPlayerCount = Playlist ? Playlist->MaxPlayers : 100;
	int32 TeamSize = Playlist ? Playlist->MaxTeamSize : FortGameState->TeamSize;

	GameSession->MaxPlayers = MaxPlayerCount;
	GameSession->MaxPartySize = TeamSize;

	MaxPlayerCount = MaxPlayerCount;

	if (Playlist) {
		if (FortGameState->_HasTeamSize()) {
			FortGameState->TeamSize = Playlist->MaxTeamSize;
		}

		if (FortGameState->_HasTeamCount()) {
			FortGameState->TeamCount = Playlist->MaxTeamCount;
		}

		if (FortGameState->_HasCurrentPlaylistData()) {
			FortGameState->CurrentPlaylistData = Playlist;
			FortGameState->OnRep_CurrentPlaylistData();
		}
		if (FortGameState->_HasCurrentPlaylistInfo()) {
			FortGameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
			FortGameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
			FortGameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
			FortGameState->CurrentPlaylistInfo.MarkArrayDirty();
			FortGameState->OnRep_CurrentPlaylistInfo();
		}

		SetCurrentPlaylistName(Playlist->GetPlaylistName());
		SetCurrentPlaylistId(Playlist->GetPlaylistId());
	}

	FortGameState->SetCurrentPlaylistId(CurrentPlaylistId);

	if (Playlist && Playlist->MaxTeamCount > 0 && Playlist->MaxTeamSize > 0) {
		bool bHasUsableTeams = false;
		for (AFortTeamInfo* TeamInfo : FortGameState->Teams) {
			if (TeamInfo && TeamInfo->Team >= 2) {
				bHasUsableTeams = true;
				break;
			}
		}

		// Some playlists can be applied after native team initialization has
		// already run, leaving only the fallback team in GameState->Teams.
		if (!bHasUsableTeams) {
			Log("AFortGameModeAthena::SetupPlaylist: Initializing missing player teams");
			InitializeTeams();
		}
	}

	Log("AFortGameModeAthena::SetupPlaylist: Applied playlist " + (Playlist ? Playlist->GetFName().ToString().ToString() : std::to_string(CurrentPlaylistId)));

	// Start Playlist Dump
	Log("====== Playlist Dump ======");
	if (_HasCurrentPlaylistName()) {
		Log("CurrentPlaylistName: " + CurrentPlaylistName.ToString().ToString());
	}
	if (_HasCurrentPlaylistId()) {
		Log("CurrentPlaylistId: " + std::to_string(CurrentPlaylistId));
	}
	if (FortGameState->_HasCurrentPlaylistId()) {
		Log("GameState CurrentPlaylistId: " + std::to_string(FortGameState->CurrentPlaylistId));
	}
	if (FortGameState->_HasTeamCount()) {
		Log("TeamCount: " + std::to_string(FortGameState->TeamCount));
	}
	if (FortGameState->_HasTeamSize()) {
		Log("TeamSize: " + std::to_string(FortGameState->TeamSize));
	}
	if (MaxPlayerCount) {
		Log("MaxPlayerCount: " + std::to_string(MaxPlayerCount));
	}
	if (FortGameState->_HasCachedSafeZoneStartUp()) {
		Log("CachedSafeZoneStartUp: " + std::to_string(FortGameState->CachedSafeZoneStartUp));
	}
	if (FortGameState->_HasbIsLargeTeamGame()) {
		Log("bIsLargeTeamGame: " + std::string(FortGameState->bIsLargeTeamGame ? "true" : "false"));
	}
	if (FortGameState->_HasAirCraftBehavior()) {
		Log("AirCraftBehavior: " + std::to_string(FortGameState->AirCraftBehavior));
	}
	Log("====== End Playlist Dump ======");

	return true;
}

void AFortGameModeAthena::FinishWorldInitialization(AFortGameModeAthena* This, AFortWorldManager* WorldManager) {
	FinishWorldInitializationOG(This, WorldManager);
	AFortGameModeZone::FinishWorldInitialization(This, WorldManager);

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::FinishWorldInitialization: GameState is null or not AFortGameStateAthena");
		return;
	}

	This->DefaultPawnClass = (UClass*)StaticLoadObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
	//This->PlayerControllerClass = (UClass*)StaticLoadObject("/Game/Athena/Athena_PlayerController.Athena_PlayerController_C");

	This->bDisableGCOnServerDuringMatch = true;

	if (GameState->MapInfo) {
		GameState->MapInfo->SpawnLlamas();
	}
	else {
		Log("AFortGameModeAthena::FinishWorldInitialization: MapInfo is null");
	}

	if (UAthenaBattleBusItemDefinition* BBID = GetBattleBusItemDefinition()) {
		GameState->DefaultBattleBus = BBID;
		Log("Set Custom Battle Bus: " + BBID->GetName().ToString());
	}
	if (UClass* SupplyDropClass = GetSupplyDropClass()) {
		GameState->MapInfo->SupplyDropClass = SupplyDropClass;

		for (UFortSupplyDropInfo* SupplyDropInfo : GameState->MapInfo->SupplyDropInfoList) {
			SupplyDropInfo->SupplyDropClass = SupplyDropClass;
		}

		Log("Set Custom Supply Drop Class: " + SupplyDropClass->GetName().ToString());
	}
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
		return 2;
	}
	if (Config.bUseGameSessions) {
		return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
	}

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::PickTeam: GameState is null or not AFortGameStateAthena");
		return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
	}

	/*for (int32 i = 0; i < GameState->Teams.Num(); i++) {
		AFortTeamInfo* TeamInfo = GameState->Teams[i];
		if (!TeamInfo) {
			continue;
		}
		
		Log("AFortGameModeAthena::PickTeam: Team " + TeamInfo->GetName().ToString());
		Log("AFortGameModeAthena::PickTeam: Team " + std::to_string(TeamInfo->Team) + " has " + std::to_string(TeamInfo->TeamMembers.Num()) + " members.");
	}*/

	APlayerState* PlayerStateToIgnore = ControllerToPickFor ? ControllerToPickFor->PlayerState : nullptr;

	for (AFortTeamInfo* TeamInfo : GameState->Teams) {
		if (!TeamInfo || TeamInfo->Team < 2) {
			continue;
		}

		int32 TeamMemberCount = 0;
		for (APlayerState* PlayerState : GameState->PlayerArray) {
			if (!PlayerState || PlayerState == PlayerStateToIgnore) {
				continue;
			}

			AFortPlayerStateAthena* AthenaPlayerState = PlayerState->Cast<AFortPlayerStateAthena>();
			if (AthenaPlayerState && AthenaPlayerState->TeamIndex == TeamInfo->Team) {
				TeamMemberCount++;
			}
		}

		if (TeamMemberCount < GameState->TeamSize) {
			Log("AFortGameModeAthena::PickTeam: Assigning player to team " + std::to_string(TeamInfo->Team) + " with " + std::to_string(TeamMemberCount) + " members.");
			return TeamInfo->Team;
		}
	}

	Log("AFortGameModeAthena::PickTeam: Failed to find a team!");
	return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
}

void AFortGameModeAthena::InitGameState(AFortGameModeAthena* This) {
	InitGameStateOG(This);

	if (Version::Fortnite_Version >= 3.5 && Version::Fortnite_Version <= 3.6) {
		std::thread([This]() {
			while (true) {
				UWorld* World = UWorld::GetWorld();
				if (World && World->AuthorityGameMode) {
					AGameMode* GameMode = World->AuthorityGameMode->Cast<AGameMode>();
					if (GameMode && GameMode->MatchState == MatchState::WaitingToStart) {
						break;
					}
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			// sleep for a second to make sure it wont freeze
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			if (!This || !This->SetupPlaylist()) {
				Log("Failed to setup playlist!");
			}
		}).detach();
	}
	else {
		if (!This->SetupPlaylist()) {
			Log("AFortGameModeAthena::InitGameState: Failed to setup playlist");
		}
	}
}

UAthenaBattleBusItemDefinition* AFortGameModeAthena::GetBattleBusItemDefinition() {
	UAthenaBattleBusItemDefinition* BBID = nullptr;
	if (Version::Fortnite_Version == 1.11
		|| (Version::Fortnite_Version >= 2.1 && Version::Fortnite_Version <= 2.42)) {
		BBID = (UAthenaBattleBusItemDefinition*)StaticLoadObject("/Game/Athena/Items/Cosmetics/BattleBuses/BBID_WinterBus.BBID_WinterBus");
	}

	return BBID;
}

UClass* AFortGameModeAthena::GetSupplyDropClass() {
	UClass* SupplyDropClass = nullptr;
	if (Version::Fortnite_Version == 1.11
		|| (Version::Fortnite_Version >= 2.1 && Version::Fortnite_Version <= 2.42)) {
		SupplyDropClass = (UClass*)StaticLoadObject("/Game/Athena/SupplyDrops/B_AthenaSupplyDrop_Gift.B_AthenaSupplyDrop_Gift_C");
	}

	return SupplyDropClass;
}

void AFortGameModeAthena::PlacePlayerOnTeam(AFortGameModeAthena* This, AFortPlayerController* FortPC) {
	PlacePlayerOnTeamOG(This, FortPC);
	if (!FortPC) {
		return;
	}

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::PlacePlayerOnTeam: GameState is null or not AFortGameStateAthena");
		return;
	}

	AFortPlayerStateAthena* FortPS = FortPC->PlayerState->Cast<AFortPlayerStateAthena>();
	if (!FortPS) {
		return;
	}

	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (FortPS->_HasSquadId()) {
		if (!Config.bUseGameSessions) {
			FortPS->SquadId = FortPS->TeamIndex;
		}

		FortPS->OnRep_SquadId();
		Log("AFortGameModeAthena::PlacePlayerOnTeam: SquadId for PlayerState: " + FortPS->GetName().ToString() + " is " + std::to_string(FortPS->SquadId));
	}
}

void AFortGameModeAthena::PreInitializeComponents(AFortGameModeAthena* This) {
	PreInitializeComponentsOG(This);

	Log("AFortGameModeAthena::PreInitializeComponents");

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::PreInitializeComponents: GameState is null or not AFortGameStateAthena");
		return;
	}
}