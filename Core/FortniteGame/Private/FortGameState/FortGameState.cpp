#include "pch.h"
#include "FortniteGame/Public/FortGameState/FortGameState.h"

#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"

void AFortGameState::OnRep_AdditionalPlaylistLevelsStreamed()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_AdditionalPlaylistLevelsStreamed"));

	ProcessEvent(Func, nullptr);
}

void AFortGameState::OnFinishedStreamingAdditionalPlaylistLevel()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnFinishedStreamingAdditionalPlaylistLevel"));

	ProcessEvent(Func, nullptr);
}

void AFortGameState::ApplyHomebaseEffectsOnPlayerSetup(AFortGameState* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp)
{
	ApplyHomebaseEffectsOnPlayerSetupOG(This, SourceAccountID, McpProfile, AbilityObject, Hero, bApplyTeamEffect, bApplyTeamEffectToOtherPlayers, bIgnoreStatClamp);
}

void AFortGameState::Hook()
{
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetup()), ApplyHomebaseEffectsOnPlayerSetup, (LPVOID*)&ApplyHomebaseEffectsOnPlayerSetupOG);

	Log("Hooked AFortGameState");
}