#include "pch.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"

void AFortGameStateAthena::OnRep_CurrentPlaylistInfo() {
	if (Finder::FindAFortGameStateAthena_OnRep_CurrentPlaylistInfo()) {
		void (*OnRep_CurrentPlaylistInfoInternal)(AFortGameStateAthena*) = decltype(OnRep_CurrentPlaylistInfoInternal)(ImageBase + Finder::FindAFortGameStateAthena_OnRep_CurrentPlaylistInfo());
		OnRep_CurrentPlaylistInfoInternal(this);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_CurrentPlaylistInfo"));

		ProcessEvent(Func, nullptr);
	}
}

void AFortGameStateAthena::OnRep_CurrentPlaylistId()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_CurrentPlaylistId"));

	ProcessEvent(Func, nullptr);
}

void AFortGameStateAthena::OnPlaylistDataLoadCompleted() {
	void (*OnPlaylistDataLoadCompletedInternal)(AFortGameStateAthena*) = decltype(OnPlaylistDataLoadCompletedInternal)(ImageBase + Finder::FindAFortGameStateAthena_OnPlaylistDataLoadCompleted());
	OnPlaylistDataLoadCompletedInternal(this);
}

void AFortGameStateAthena::UpdatePlaylistDependentData() {
	void (*UpdatePlaylistDependentDataInternal)(AFortGameStateAthena*) = decltype(UpdatePlaylistDependentDataInternal)(ImageBase + Finder::FindAFortGameStateAthena_UpdatePlaylistDependentData());
	UpdatePlaylistDependentDataInternal(this);
}

void AFortGameStateAthena::SetCurrentPlaylistId(int InPlaylistId) {
	if (Finder::FindAFortGameStateAthena_SetCurrentPlaylistId()) {
		void (*SetCurrentPlaylistIdInternal)(AFortGameStateAthena* This, int InPlaylistId) = decltype(SetCurrentPlaylistIdInternal)(ImageBase + Finder::FindAFortGameStateAthena_SetCurrentPlaylistId());
		SetCurrentPlaylistIdInternal(this, InPlaylistId);
	}
	else {
		CurrentPlaylistId = InPlaylistId;
	}
}