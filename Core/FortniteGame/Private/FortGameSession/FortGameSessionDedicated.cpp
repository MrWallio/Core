#include "pch.h"
#include "FortniteGame/Public/FortGameSession/FortGameSessionDedicated.h"

bool AFortGameSessionDedicated::FinalizeCreation() {
	bool (*&FinalizeCreationInternal)(AFortGameSessionDedicated * This) = decltype(FinalizeCreationInternal)(VTable[Finder::FindAFortGameSessionDedicated_FinalizeCreationVFT()]);
	return FinalizeCreationInternal(this);
}