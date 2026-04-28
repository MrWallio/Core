#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Channel.h"

class UActorChannel : public UChannel {
public:
	DefineUnrealClass(UActorChannel);

	DefineUProperty(AActor*, Actor);

	DefineCustomProperty(double, RelevantTime, ServerOffsets::UActorChannel__RelevantTime);

	DefineCustomProperty(double, LastUpdateTime, ServerOffsets::UActorChannel__LastUpdateTime);
public:
	void SetChannelActorForDestroy(struct FActorDestructionInfo* DestructInfo);

	void SetChannelActor(class AActor* InActor);

	bool ReplicateActor();
};

class UActorChannelUE416 : public UChannelUE416
{
public:
	// Variables.
	AActor* Actor; // Actor this corresponds to.

	FNetworkGUID	ActorNetGUID; // Actor GUID (useful when we don't have the actor resolved yet). Currently only valid on clients.
	float			CustomTimeDilation;

	// Variables.
	double	RelevantTime; // Last time this actor was relevant to client.
	double	LastUpdateTime;	// Last time this actor was replicated.
	uint32  SpawnAcked : 1;	// Whether spawn has been acknowledged.
	uint32  bForceCompareProperties : 1; // Force this actor to compare all properties for a single frame
	uint32  bIsReplicatingActor : 1; // true when in this channel's ReplicateActor() to avoid recursion as that can cause invalid data to be sent

	/** whether we should nullptr references to this channel's Actor in other channels' Recent data when this channel is closed
	 * set to false in cases where the Actor can't become relevant again (e.g. destruction) as it's unnecessary in that case
	 */
	uint32 bClearRecentActorRefs : 1;

	uint8 Padding[0x198];
};