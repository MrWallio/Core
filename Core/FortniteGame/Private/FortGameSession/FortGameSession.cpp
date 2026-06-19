#include "pch.h"
#include "FortniteGame/Public/FortGameSession/FortGameSession.h"

EFortPlayerValidationType AFortGameSession::ValidatePlayer(AFortGameSession* This, const FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason) {
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	
	if (Config.bUseGameSessions && !Config.bSkipSessionValidation) {
		return ValidatePlayerOG(This, UniqueId, bIsLocalPlayer, ReturnReason);
	}
	else {
		return EFortPlayerValidationType::ValidatedPlayer;
	}
}

void AFortGameSession::CreateServerGame() {
	void (*CreateServerGameInternal)(AFortGameSession * This) = decltype(CreateServerGameInternal)(ImageBase + Finder::FindAFortGameSession_CreateServerGame());
	CreateServerGame();
}