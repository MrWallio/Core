#pragma once
#include "pch.h"

#include "FortGamePvPBase.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"

class AFortPlayerControllerAthena;
struct FVector;
class AFortAIDirector;
class UFortPlaylistAthena;
class AFortWorldManager;

class AFortGameModeAthena : public AFortGamePvPBase {
public:
	DefineUnrealClass(AFortGameModeAthena);

	DefineUProperty(int32, WarmupRequiredPlayerCount);

	DefineUProperty(float, PlaylistHotfixOriginalGCFrequency);
	DefineUProperty(bool, bDisableGCOnServerDuringMatch);
	DefineUProperty(bool, bPlaylistHotfixChangedGCDisabling);

	DefineUProperty(TArray<AFortPlayerControllerAthena*>, AlivePlayers);
public:
	static inline bool (*ReadyToStartMatchOG)(AFortGameModeAthena* This);
	bool ReadyToStartMatch();
	static bool ReadyToStartMatchHK(AFortGameModeAthena* This);

	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);
	APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot);
	static APawn* SpawnDefaultPawnForHK(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);

	static inline void (*FinishWorldInitializationOG)(AFortGameModeAthena* This, AFortWorldManager* WorldManager);
	void FinishWorldInitialization(AFortWorldManager* WorldManager);
	static void FinishWorldInitializationHK(AFortGameModeAthena* This, AFortWorldManager* WorldManager);

	static inline void (*BeginPlayOG)(AFortGameModeAthena* This);
	void BeginPlay();
	static void BeginPlayHK(AFortGameModeAthena* This);

	void AddToAlivePlayers(AFortPlayerControllerAthena* PC);

	static inline int32 (*StartAircraftPhaseOG)(AFortGameModeAthena* This, bool bGoStraightToSafeZone);
	int32 StartAircraftPhase(bool bGoStraightToSafeZone);
	static int32 StartAircraftPhaseHK(AFortGameModeAthena* This, bool bGoStraightToSafeZone);

	static void Hook() {
		//MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_ReadyToStartMatch()), ReadyToStartMatch, (LPVOID*)&ReadyToStartMatchOG);
		HookEveryVTable(
			AFortGameModeAthena::StaticClass(),
			AFortGameModeAthena::StaticClass()->GetFunction("Function /Script/Engine.GameMode.ReadyToStartMatch"),
			ReadyToStartMatchHK,
			(LPVOID*)&ReadyToStartMatchOG
		);

		//MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_SpawnDefaultPawnFor()), SpawnDefaultPawnFor, (LPVOID*)&SpawnDefaultPawnForOG);
		HookEveryVTable(
			AFortGameModeAthena::StaticClass(),
			AFortGameModeAthena::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"),
			SpawnDefaultPawnForHK,
			(LPVOID*)&SpawnDefaultPawnForOG
		);

		//MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_FinishWorldInitialization()), FinishWorldInitialization, (LPVOID*)&FinishWorldInitializationOG);
		HookEveryVTableIdx(
			AFortGameModeAthena::StaticClass(),
			Finder::FindAFortGameMode_FinishWorldInitializationVFT(),
			FinishWorldInitializationHK,
			(LPVOID*)&FinishWorldInitializationOG
		);

		HookVTableIdx(
			AFortGameModeAthena::GetDefaultObj(),
			Finder::FindAActor_BeginPlayVFT(),
			BeginPlayHK,
			(LPVOID*)&BeginPlayOG
		);

		if (Finder::FindAFortGameModeAthena_StartAircraftPhase()) {
			MH_CreateHook(
				(LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_StartAircraftPhase()),
				StartAircraftPhaseHK,
				(LPVOID*)&StartAircraftPhaseOG
			);
		}

		Log("Hooked AFortGameModeAthena");
	}
};