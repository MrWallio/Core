#include "pch.h"
#include "FortniteGame/Public/FortGameSession/FortGameSession.h"

EFortPlayerValidationType AFortGameSession::ValidatePlayer(AFortGameSession* This, const FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason) {
	if (ConfigurationManager::GetConfig().bUseGameSessions) {
		return ValidatePlayerOG(This, UniqueId, bIsLocalPlayer, ReturnReason);
	}
	else {
		return EFortPlayerValidationType::ValidatedPlayer;
	}
}