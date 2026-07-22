#pragma once
#include "pch.h"

#include "FortPlaylist.h"
#include "FortniteGame/Public/FortEnums.h"

class UAthenaAISettings;

class UFortPlaylistAthena : public UFortPlaylist {
public:
	DefineUnrealClass(UFortPlaylistAthena);

	DefineUProperty(uint8, SafeZoneStartUp);
	DefineUProperty(bool, bIsLargeTeamGame);
	DefineUProperty(EAthenaRespawnType, RespawnType);
	DefineUProperty(uint8, AirCraftBehavior);
	DefineUProperty(UAthenaAISettings*, AISettings);
	DefineUProperty(bool, bUseSameDirectionForOpposingAircraft);
};