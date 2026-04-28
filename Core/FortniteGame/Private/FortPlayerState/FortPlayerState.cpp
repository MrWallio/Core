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
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_HeroType"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&OnRep_HeroTypeInternal)(AFortPlayerState*) = decltype(OnRep_HeroTypeInternal)(VTable[VTableIdx]);
		return OnRep_HeroTypeInternal(this);
	}
}

void AFortPlayerState::InitializeHero() {
	void (*InitializeHeroInternal)(AFortPlayerState*) = decltype(InitializeHeroInternal)(ImageBase + Finder::FindAFortPlayerState_InitializeHero());
	InitializeHeroInternal(this);
}