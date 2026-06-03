#pragma once
#include "pch.h"

#include "FortGameState.h"

class AFortGameStateZone : public AFortGameState {
public:
	DefineUnrealClass(AFortGameStateZone);

	DefineUProperty(float, GameDifficulty);
public:
	void OnRep_GameDifficulty();

	static void Hook();
};