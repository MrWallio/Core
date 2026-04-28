#pragma once
#include "pch.h"

#include "FortGameState.h"

class AFortGameStateZone : public AFortGameState {
public:
	DefineUnrealClass(AFortGameStateZone);
public:
	static void Hook();
};