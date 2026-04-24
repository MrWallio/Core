#pragma once
#include "pch.h"

/** Types of network failures broadcast from the engine */
namespace ENetworkFailure
{
	enum Type : int
	{
		/** A relevant net driver has already been created for this service */
		NetDriverAlreadyExists,
		/** The net driver creation failed */
		NetDriverCreateFailure,
		/** The net driver failed its Listen() call */
		NetDriverListenFailure,
		/** A connection to the net driver has been lost */
		ConnectionLost,
		/** A connection to the net driver has timed out */
		ConnectionTimeout,
		/** The net driver received an NMT_Failure message */
		FailureReceived,
		/** The client needs to upgrade their game */
		OutdatedClient,
		/** The server needs to upgrade their game */
		OutdatedServer,
		/** There was an error during connection to the game */
		PendingConnectionFailure,
		/** NetGuid mismatch */
		NetGuidMismatch,
		/** Network checksum mismatch */
		NetChecksumMismatch
	};
}