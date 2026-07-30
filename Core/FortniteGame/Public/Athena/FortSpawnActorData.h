#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Athena/FortSpawnActorInfo.h"


struct FFortSpawnActorData {
public:
	DefineUnrealStruct(FFortSpawnActorData);

	DefineStructProperty(UFortSpawnActorInfo*, SpawnActorInfo);
	DefineStructProperty(int32, NumSpawnsRemaining);
	DefineStructProperty(float, TimeUntilNextSpawn);
	DefineStructProperty(TArray<class AActor*>, SpawnedFortSpawnActors);
};

