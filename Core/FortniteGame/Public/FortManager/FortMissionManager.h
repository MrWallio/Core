#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class AFortBluGloManager;

class UFortMissionInfo : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortMissionInfo);
};

class AFortMissionManager : public AActor {
public:
	DefineUnrealClass(AFortMissionManager);

	DefineUProperty(TSoftObjectPtr<UClass>, BluGloManagerClass);
	DefineUProperty(AFortBluGloManager*, BluGloManager);
};