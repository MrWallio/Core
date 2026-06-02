#include "pch.h"
#include "FortniteGame/Public/FortGameSession/FortGameSession.h"

EFortPlayerValidationType AFortGameSession::ValidatePlayer(FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason) {
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	
	if (Config.bUseGameSessions && !Config.bSkipSessionValidation) {
		return ValidatePlayerOG(this, UniqueId, bIsLocalPlayer, ReturnReason);
	}
	else {
		return EFortPlayerValidationType::ValidatedPlayer;
	}
}

EFortPlayerValidationType AFortGameSession::ValidatePlayerHK(AFortGameSession* This, FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason) {
	if (!This) return ValidatePlayerOG(This, UniqueId, bIsLocalPlayer, ReturnReason);

	return This->ValidatePlayer(UniqueId, bIsLocalPlayer, ReturnReason);
}