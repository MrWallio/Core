#pragma once
#include "pch.h"

#include "FortPlayerControllerGameplay.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

class AFortPlayerPawnAthena;

class AFortPlayerControllerZone : public AFortPlayerControllerGameplay {
public:
	DefineUnrealClass(AFortPlayerControllerZone);
public:
	static void ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P);

	static void Hook();
};