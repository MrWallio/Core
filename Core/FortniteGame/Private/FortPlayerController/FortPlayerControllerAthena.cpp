#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"

void AFortPlayerControllerAthena::EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft) {
	EnterAircraftOG(This, InAircraft);

	if (This->WorldInventory) {
		This->WorldInventory->DropAllItems(false);
	}
}

void AFortPlayerControllerAthena::ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation) {
	ServerAttemptAircraftJumpOG(This, ClientRotation);

	Log("ServerAttemptAircraftJump Called.");

	if (This->WorldInventory) {
		if (This->IsUsingOldQuickBars()) {
			This->WorldInventory->EquipHarvestingTool();
		}
	}

	if (This->MyFortPawn) {
		AFortPlayerPawn* PlayerPawn = This->MyFortPawn;

		PlayerPawn->SetHealth(PlayerPawn->GetMaxHealth());
		PlayerPawn->SetShield(0.0f);
	}
}