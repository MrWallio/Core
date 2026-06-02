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

bool AFortGameModeAthena::ReadyToStartMatch() {
	if (bWorldIsReady
		&& MatchState == MatchState::WaitingToStart
		&& NumPlayers >= WarmupRequiredPlayerCount) {

		Log(std::format("ReadyToStartMatch: Match ready to start! Players={}/{}, WorldReady={}",
			NumPlayers,
			WarmupRequiredPlayerCount,
			bWorldIsReady));
		return true;
	}

	return false;
}

bool AFortGameModeAthena::ReadyToStartMatchHK(AFortGameModeAthena* This) {
	if (!This) {
		return ReadyToStartMatchOG(This);
	}

	return This->ReadyToStartMatch();
}

APawn* AFortGameModeAthena::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) {
	APawn* Pawn = AFortGameModeZone::SpawnDefaultPawnFor(this, NewPlayer, StartSpot);

	Log("AFortGameModeAthena::SpawnDefaultPawnFor: Spawned default pawn. NewPlayer=" + (NewPlayer ? NewPlayer->GetName().ToString() : "None") + " Pawn=" + (Pawn ? Pawn->GetName().ToString() : "None"));
	return Pawn;
}

APawn* AFortGameModeAthena::SpawnDefaultPawnForHK(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot) {
	if (!This) {
		return SpawnDefaultPawnForOG(This, NewPlayer, StartSpot);
	}

	return This->SpawnDefaultPawnFor(NewPlayer, StartSpot);
}

void AFortGameModeAthena::FinishWorldInitialization(AFortWorldManager* WorldManager) {
	AFortGameStateAthena* GameState = GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("FinishWorldInitialization: GameState is null or not AFortGameStateAthena");
		return FinishWorldInitializationOG(this, WorldManager);
	}
	
	GameState->SetCurrentPlaylistId(CurrentPlaylistId);

	CreateAIDirector();
	CreateAIGoalManager();

	FinishWorldInitializationOG(this, WorldManager);
	bWorldIsReady = true;
}

void AFortGameModeAthena::FinishWorldInitializationHK(AFortGameModeAthena* This, AFortWorldManager* WorldManager) {
	if (!This) {
		return FinishWorldInitializationOG(This, WorldManager);
	}

	return This->FinishWorldInitialization(WorldManager);
}

void AFortGameModeAthena::BeginPlay() {
	BeginPlayOG(this);

	if (Version::Fortnite_Version <= 1.72) {
		DefaultPawnClass = (UClass*)StaticLoadObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
		PlayerControllerClass = (UClass*)StaticLoadObject("/Game/Athena/Athena_PlayerController.Athena_PlayerController_C");
		PlayerStateClass = AFortPlayerStateAthena::StaticClass();
	}

	bDisableGCOnServerDuringMatch = true;
}

void AFortGameModeAthena::BeginPlayHK(AFortGameModeAthena* This) {
	if (!This) {
		return BeginPlayOG(This);
	}

	return This->BeginPlay();
}

void AFortGameModeAthena::AddToAlivePlayers(AFortPlayerControllerAthena* PC) {
	void (*AddToAlivePlayersInternal)(AFortGameModeAthena* This, AFortPlayerControllerAthena* PC) = decltype(AddToAlivePlayersInternal)(ImageBase + Finder::FindAFortGameModeAthena_AddToAlivePlayers());
	AddToAlivePlayersInternal(this, PC);
}

int32 AFortGameModeAthena::StartAircraftPhase(bool bGoStraightToSafeZone) {
	for (AFortPlayerControllerAthena* PC : AlivePlayers) {
		if (PC->WorldInventory) {
			PC->WorldInventory->DropAllItems(false);
		}

		if (PC->MyFortPawn || PC->Pawn->Cast<AFortPlayerPawn>()) {
			AFortPlayerPawn* MyFortPawn = PC->MyFortPawn ? PC->MyFortPawn : PC->Pawn->Cast<AFortPlayerPawn>();

			MyFortPawn->SetHealth(MyFortPawn->GetMaxHealth());
			MyFortPawn->SetShield(0.0f);
		}
	}

	return StartAircraftPhaseOG(this, bGoStraightToSafeZone);
}

int32 AFortGameModeAthena::StartAircraftPhaseHK(AFortGameModeAthena* This, bool bGoStraightToSafeZone) {
	if (!This) {
		return StartAircraftPhaseOG(This, bGoStraightToSafeZone);
	}

	return This->StartAircraftPhase(bGoStraightToSafeZone);
}