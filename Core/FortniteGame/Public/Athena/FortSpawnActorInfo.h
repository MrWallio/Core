#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"



class UFortSpawnActorInfo {
public:
	DefineUnrealClass(UFortSpawnActorInfo);

	DefineUProperty(FScalableFloat, SafeZoneIndex);
	DefineUProperty(uint8, SpawnTiming);
};

