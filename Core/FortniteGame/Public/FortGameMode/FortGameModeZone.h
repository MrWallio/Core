#pragma once
#include "pch.h"

#include "FortGameMode.h"
#include "FortniteGame/Public/FortItem/ItemAndCount.h"

class AFortPlayerController;
class AFortAIGoalManager;
class AFortAIDirector;
class ABuildingSMActor;

class AFortGameModeZone : public AFortGameMode {
public:
	DefineUnrealClass(AFortGameModeZone);

	DefineUProperty(TArray<FItemAndCount>, StartingItems);

	DefineUProperty(AFortAIDirector*, AIDirector);
	DefineUProperty(AFortAIGoalManager*, AIGoalManager);
};