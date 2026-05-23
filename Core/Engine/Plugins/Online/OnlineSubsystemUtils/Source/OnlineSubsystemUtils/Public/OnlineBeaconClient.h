#pragma once
#include "pch.h"

#include "OnlineBeacon.h"

class AOnlineBeaconHostObject;

class AOnlineBeaconClient : public AOnlineBeacon {
public:
	DefineUnrealClass(AOnlineBeaconClient);

	DefineUProperty(AOnlineBeaconHostObject*, BeaconOwner);
};