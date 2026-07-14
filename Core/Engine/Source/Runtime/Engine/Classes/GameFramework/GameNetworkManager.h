#pragma once
#include "pch.h"

#include "Info.h"

enum EStandbyType
{
	STDBY_Rx,
	STDBY_Tx,
	STDBY_BadPing,
	STDBY_MAX,
};

class AGameNetworkManager : public AInfo {
public:
	DefineUnrealClass(AGameNetworkManager);

	DefineUProperty(float, MaxMoveDeltaTime);
	DefineUProperty(float, MAXPOSITIONERRORSQUARED);
	DefineUProperty(float, MAXNEARZEROVELOCITYSQUARED);
	DefineUProperty(bool, ClientAuthorativePosition);
	DefineUProperty(bool, bMovementTimeDiscrepancyDetection);
	DefineUProperty(float, StandbyRxCheatTime);
	DefineUProperty(float, StandbyTxCheatTime);
	DefineUProperty(int32, BadPingThreshold);
public:
	// impl vtable calls for these
	bool IsInLowBandwidthMode();

	void StandbyCheatDetected(EStandbyType StandbyType);
};	