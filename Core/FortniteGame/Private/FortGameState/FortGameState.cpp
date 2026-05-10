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

void AFortGameState::OnRep_CurrentWUID(AFortGameState* This)
{
	if (Version::Fortnite_Version >= 1.8) {
		Log("OnRep_CurrentWUID called!");
	}
	else {
		OnRep_CurrentWUIDOG(This);
	}
}

void AFortGameState::OnRep_WorldManager(AFortGameState* This)
{
	if (Version::Fortnite_Version >= 1.8) {
		Log("OnRep_WorldManager called!");
	}
	else {
		OnRep_WorldManagerOG(This);
	}
}

void AFortGameState::Hook()
{
	HookEveryVTable(
		AFortGameState::StaticClass(),
		AFortGameState::StaticClass()->GetFunction("Function /Script/FortniteGame.FortGameState.OnRep_CurrentWUID"),
		OnRep_CurrentWUID,
		(LPVOID*)&OnRep_CurrentWUIDOG
	);

	HookEveryVTable(
		AFortGameState::StaticClass(),
		AFortGameState::StaticClass()->GetFunction("Function /Script/FortniteGame.FortGameState.OnRep_WorldManager"),
		OnRep_WorldManager,
		(LPVOID*)&OnRep_WorldManagerOG
	);

	Log("Hooked AFortGameState");
}