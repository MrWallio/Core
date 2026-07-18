#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

class AFortAIGoalManager : public AActor {
public:
	DefineUnrealClass(AFortAIGoalManager);

	static AFortAIGoalManager* GetCurrent(UObject* WorldContextObject);

	static void Hook();
};
