#pragma once
#include "pch.h"

#include "FortGameState.h"

class AFortPlayerState;

class AFortGameStateZone : public AFortGameState {
public:
	DefineUnrealClass(AFortGameStateZone);

	DefineUProperty(float, GameDifficulty);
	DefineUProperty(bool, bDBNODeathEnabled);
public:
	void OnRep_GameDifficulty();

	bool IsRespawningAllowed(AFortPlayerState* PlayerState);

	static void Hook();
};