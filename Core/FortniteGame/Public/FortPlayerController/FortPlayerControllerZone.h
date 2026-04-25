#pragma once
#include "pch.h"

#include "FortPlayerControllerGameplay.h"

class AFortPlayerPawnAthena;

class AFortPlayerControllerZone : public AFortPlayerControllerGameplay {
public:
	DefineUnrealClass(AFortPlayerControllerZone);
public:
	static void ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P);

	static void Hook();
};