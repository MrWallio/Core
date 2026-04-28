#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Public/Net/DataBunch.h"

class FInBunch;
class FOutBunch;
class UNetConnection;

enum EChannelType
{
    CHTYPE_None = 0,  // Invalid type.
    CHTYPE_Control = 1,  // Connection control.
    CHTYPE_Actor = 2,  // Actor-update channel.
    CHTYPE_File = 3,  // Binary file transfer.
    CHTYPE_Voice = 4,  // VoIP data channel
    CHTYPE_MAX = 8,  // Maximum.
};

class UChannel : public UObject {
public:
    DefineUnrealClass(UChannel);

    DefineUProperty(UNetConnection*, Connection);

    DefineCustomProperty(int32, NumInRec, ServerOffsets::UChannel__NumInRec);
    DefineCustomProperty(int32, NumOutRec, ServerOffsets::UChannel__NumOutRec);
public:
    int32 IsNetReady(bool Saturate);

    FPacketIdRange SendBunch(FOutBunch* Bunch, bool Merge);

	void Close();

	void StartBecomingDormant();
};

// UE 4.16 - 4.20
class UChannelUE416 : public UChannel
{
public:
	UNetConnection* Connection; // Owner connection.

	// Variables.
	uint32 OpenAcked : 1;		// If OpenedLocally is true, this means we have acknowledged the packet we sent the bOpen bunch on. Otherwise, it means we have received the bOpen bunch from the server.
	uint32 Closing : 1;			// State of the channel.
	uint32 Dormant : 1;			// Channel is going dormant (it will close but the client will not destroy
	uint32 bIsReplicationPaused : 1;	// Replication is being paused, but channel will not be closed
	uint32 OpenTemporary : 1;	// Opened temporarily.
	uint32 Broken : 1;			// Has encountered errors and is ignoring subsequent packets.
	uint32 bTornOff : 1;			// Actor associated with this channel was torn off
	uint32 bPendingDormancy : 1;	// Channel wants to go dormant (it will check during tick if it can go dormant)
	uint32 bPausedUntilReliableACK : 1; // Unreliable property replication is paused until all reliables are ack'd.
	int32 ChIndex;			// Index of this channel.
	int32 OpenedLocally;		// Whether channel was opened locally or by remote.
	FPacketIdRange OpenPacketId;		// If OpenedLocally is true, this is the packet we sent the bOpen bunch on. Otherwise, it's the packet we received the bOpen bunch on.
	EChannelType ChType;				// Type of this channel.
	int32 NumInRec;			// Number of packets in InRec.
	int32 NumOutRec;			// Number of packets in OutRec.
	FInBunch* InRec; // Incoming data with queued dependencies.
	FOutBunch* OutRec; // Outgoing reliable unacked data.
	FInBunch* InPartialBunch;
};