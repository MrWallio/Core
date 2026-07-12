#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class UFortPersonalVehicleItem;

struct FFortCampaignLoadout
{
public:
	DefineUnrealStruct(FFortCampaignLoadout);

	DefineStructProperty(UFortPersonalVehicleItem*, PersonalVehicle);
};