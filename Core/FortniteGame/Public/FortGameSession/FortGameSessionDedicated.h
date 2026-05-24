#pragma once
#include "pch.h"

#include "FortGameSession.h"

class AFortGameSessionDedicated : public AFortGameSession {
public:
	DefineUnrealClass(AFortGameSessionDedicated);

	DefineUProperty(float, HotfixCheckTimer);
public:
	bool FinalizeCreation();

	static inline void (*OnUpdateCompleteOG)(AFortGameSessionDedicated* This, uint8 Result, void* Callback);
	static void OnUpdateComplete(AFortGameSessionDedicated* This, uint8 Result, void* Callback);

	static void Hook() {
		PatchByte(ImageBase + Finder::FindAFortGameSessionDedicated_FinalizeCreationPatch1(), 0x85);

		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameSessionDedicated_OnUpdateComplete()), OnUpdateComplete, (LPVOID*)&OnUpdateCompleteOG);

		Log("AFortGameSessionDedicated Hooked!");
	}
};