#pragma once
#include "pch.h"

#include "FortPlayerStateZone.h"

class AFortPlayerStateAthena final : public AFortPlayerStateZone {
public:
	DefineUnrealClass(AFortPlayerStateAthena);

	DefineUProperty(uint8, TeamIndex);
};