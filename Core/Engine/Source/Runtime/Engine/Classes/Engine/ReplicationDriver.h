#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AActor;
class UActorChannel;
class UNetConnection;
class UNetDriver;
class UReplicationDriver;
class UWorld;

struct FActorDestructionInfo;
struct FURL;

class UReplicationDriver : public UObject {
public:
	DefineUnrealClass(UReplicationDriver);
public:
	int32 ServerReplicateActors(float DeltaSeconds) {
		if (Finder::FindUReplicationDriver_ServerReplicateActorsVFT()) {
			int32(*&ServerReplicateActorsInternal)(UReplicationDriver*, float) = decltype(ServerReplicateActorsInternal)(VTable[Finder::FindUReplicationDriver_ServerReplicateActorsVFT()]);
			return ServerReplicateActorsInternal(this, DeltaSeconds);
		}
		else {
			return 0;
		}
	}
};