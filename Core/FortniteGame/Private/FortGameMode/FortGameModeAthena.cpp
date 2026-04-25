#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortWorld/FortWorldManager.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"

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

	AFortPlayerPawnAthena* Pawn = (AFortPlayerPawnAthena*)((AFortGameMode*)This)->SpawnDefaultPawnFor(NewPlayer, StartSpot);

	if (Version::Fortnite_Version <= 1.72) {
		NewPlayer->ClientForceProfileQuery();

		UCustomCharacterPart* HeadPart = (UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
		UCustomCharacterPart* BodyPart = (UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

		Pawn->ServerChoosePart(EFortCustomPartType::Head, HeadPart);
		Pawn->ServerChoosePart(EFortCustomPartType::Body, BodyPart);

		Log("HeadPart: " + std::string(HeadPart ? HeadPart->GetName().ToString() : "None"));
		Log("BodyPart: " + std::string(BodyPart ? BodyPart->GetName().ToString() : "None"));

		PlayerState->OnRep_CharacterParts();
		PlayerState->OnRep_HeroType();

		FString HeroId = PlayerState->HeroType->GetName();
		Log("HeroId: " + HeroId.ToString());
		PlayerState->HeroId = HeroId;

		PlayerState->InitializeHero();
	}
	else {
		// ApplyCharacterCustomization
	}

	NewPlayer->PlayerState->ForceNetUpdate();
	Pawn->ForceNetUpdate();
	NewPlayer->ForceNetUpdate();

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

	FinishWorldInitializationOG(This, WorldManager);
	This->bWorldIsReady = true;
}