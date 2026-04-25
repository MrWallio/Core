#include "pch.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerState.h"

void AFortPlayerState::OnRep_bHasStartedPlaying()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_bHasStartedPlaying"));

	ProcessEvent(Func, nullptr);
}

void AFortPlayerState::OnRep_CharacterParts()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_CharacterParts"));

	ProcessEvent(Func, nullptr);
}

void AFortPlayerState::OnRep_HeroType()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_HeroType"));

	ProcessEvent(Func, nullptr);
}

void AFortPlayerState::InitializeHero() {
	void (*InitializeHeroInternal)(AFortPlayerState*) = decltype(InitializeHeroInternal)(ImageBase + Finder::FindAFortPlayerState_InitializeHero());
	InitializeHeroInternal(this);
}