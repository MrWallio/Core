#pragma once
#include "pch.h"

#include "FortGameState.h"

class AFortGameStateZone : public AFortGameState {
public:
	DefineUnrealClass(AFortGameStateZone);
public:
	static inline void (*ApplyHomebaseEffectsOnPlayerSetupOG)(AFortGameStateZone* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp);
	static void ApplyHomebaseEffectsOnPlayerSetup(AFortGameStateZone* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp);

	static void Hook();
};