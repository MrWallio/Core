#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

#include "AthenaLevelInfo.h"
#include "AthenaMatchStats.h"
#include "AthenaMatchTeamStats.h"
#include "AthenaRewardResult.h"

class UAthenaPlayerMatchReport : public UObject {
public:
	DefineUnrealClass(UAthenaPlayerMatchReport);

	DefineUProperty(FAthenaLevelInfo, InitialLevelInfo);
	DefineUProperty(bool, bHasMatchStats);
	DefineUProperty(FAthenaMatchStats, MatchStats);
	DefineUProperty(bool, bHasTeamStats);
	DefineUProperty(FAthenaMatchTeamStats, TeamStats);
	DefineUProperty(bool, bHasRewards);
	DefineUProperty(FAthenaRewardResult, EndOfMatchResults);
	DefineUProperty(FAthenaRewardResult, Rewards);
};