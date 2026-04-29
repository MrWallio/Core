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

APawn* AFortGameModeAthena::SpawnDefaultPawnFor(AFortGameModeAthena* This, AFortPlayerControllerAthena* NewPlayer, AActor* StartSpot) {
	AFortPlayerStateAthena* PlayerState = NewPlayer->PlayerState ? NewPlayer->PlayerState->Cast<AFortPlayerStateAthena>() : nullptr;
	if (!PlayerState) {
		Log("SpawnDefaultPawnFor: PlayerState is null or not AFortPlayerStateAthena");
		return nullptr;
	}

	APawn* Pawn = AFortGameMode::SpawnDefaultPawnForOG(This, NewPlayer, StartSpot);

	Log("SpawnDefaultPawnFor: Spawned default pawn for player " + NewPlayer->GetName().ToString() + " Pawn: " + Pawn->GetName().ToString());
	return Pawn;
}

void AFortGameModeAthena::FinishWorldInitialization(AFortGameModeAthena* This, AFortWorldManager* WorldManager) {
	AFortGameStateAthena* GameState = This->GameState ? This->GameState->Cast<AFortGameStateAthena>() : nullptr;
	if (!GameState) {
		Log("FinishWorldInitialization: GameState is null or not AFortGameStateAthena");
		return FinishWorldInitializationOG(This, WorldManager);
	}
	
	GameState->SetCurrentPlaylistId(This->CurrentPlaylistId);

	This->CreateAIDirector();
	This->CreateAIGoalManager();

	FinishWorldInitializationOG(This, WorldManager);
	This->bWorldIsReady = true;
}