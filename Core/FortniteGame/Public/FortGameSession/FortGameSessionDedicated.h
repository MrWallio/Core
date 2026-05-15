#pragma once
#include "pch.h"

#include "FortGameSession.h"

class AFortGameSessionDedicated : public AFortGameSession {
public:
	DefineUnrealClass(AFortGameSessionDedicated);

	DefineUProperty(float, HotfixCheckTimer);
public:
	bool FinalizeCreation();

	static void Hook() {
		PatchByte(ImageBase + Finder::FindAFortGameSessionDedicated_FinalizeCreationPatch1(), 0x85);

		Log("AFortGameSessionDedicated Hooked!");
	}
};