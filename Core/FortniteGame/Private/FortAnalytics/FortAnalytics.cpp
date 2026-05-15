#include "pch.h"
#include "FortniteGame/Public/FortAnalytics/FortAnalytics.h"

void UFortAnalytics::SetGameSessionID(void** Provider, FString GameSessionID, bool bSendEventIfChanged) {
    if (!Provider || !*Provider)
        return;

    return SetGameSessionIDOG(Provider, GameSessionID, bSendEventIfChanged);
}

void UFortAnalytics::SetGameStateClassName(void** Provider, FString GameStateClassName, bool bSendEventIfChanged) {
    if (!Provider || !*Provider)
        return;

    SetGameStateClassNameOG(Provider, GameStateClassName, bSendEventIfChanged);
}
