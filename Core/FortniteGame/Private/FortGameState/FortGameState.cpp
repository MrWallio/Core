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

void AFortGameState::OnRep_MissionManager()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_MissionManager");

	ProcessEvent(Func, nullptr);
}

void AFortGameState::Hook()
{
	Log("Hooked AFortGameState");
}