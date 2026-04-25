#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

struct FAdditionalLevelStreamed
{
public:
	DefineUnrealStruct(FAdditionalLevelStreamed);

	FName LevelName;

	bool bIsServerOnly;
};