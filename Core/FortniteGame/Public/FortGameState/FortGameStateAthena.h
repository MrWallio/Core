#pragma once
#include "pch.h"

#include "FortGameStateZone.h"
#include "FortniteGame/Public/FortPlaylist/PlaylistPropertyArray.h"

class AFortAthenaMapInfo;

class AFortGameStateAthena : public AFortGameStateZone {
public:
	DefineUnrealClass(AFortGameStateAthena);

	DefineUProperty(AFortAthenaMapInfo*, MapInfo);

	DefineUProperty(FPlaylistPropertyArray, CurrentPlaylistInfo);
	DefineUProperty(int32, CurrentPlaylistId);
public:
	void OnRep_CurrentPlaylistInfo();
	void OnRep_CurrentPlaylistId();

	void OnPlaylistDataLoadCompleted();
	void UpdatePlaylistDependentData();

	void SetCurrentPlaylistId(int InPlaylistId);

	static inline void (*ApplyHomebaseEffectsOnPlayerSetupOG)(AFortGameStateAthena* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp);
	static void ApplyHomebaseEffectsOnPlayerSetup(AFortGameStateAthena* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp);

	static void Hook();
};