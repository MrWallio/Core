#pragma once
#include "pch.h"

#include "FortPlayerControllerGameplay.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

class AFortPlayerPawnAthena;
struct FFortPlayerDeathReport;

class AFortPlayerControllerZone : public AFortPlayerControllerGameplay {
public:
	DefineUnrealClass(AFortPlayerControllerZone);
public:
	void ServerAcknowledgePossession(AFortPlayerPawnAthena* P);
	static inline void (*ServerAcknowledgePossessionOG)(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P);
	static void ServerAcknowledgePossessionHK(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P);

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerControllerZone* This);
	void OnReadyToStartMatch();
	static void OnReadyToStartMatchHK(AFortPlayerControllerZone* This);

	static inline void (*ServerReturnToMainMenuOG)(AFortPlayerControllerZone* This);
	void ServerReturnToMainMenu();
	static void ServerReturnToMainMenuHK(AFortPlayerControllerZone* This);

	static void Hook();
};