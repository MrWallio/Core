#include "pch.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawn.h"

void AFortPlayerPawn::BeginSkydiving(bool bFromBus)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"BeginSkydiving"));

	struct FortPlayerPawn_BeginSkydiving final
	{
	public:
		bool bFromBus;
	};

	FortPlayerPawn_BeginSkydiving Parms{};

	Parms.bFromBus = bFromBus;

	ProcessEvent(Func, &Parms);
}

void AFortPlayerPawn::ForceFinishIncomingPickups()
{
	void (*ForceFinishIncomingPickupsInternal)(AFortPlayerPawn * This) = decltype(ForceFinishIncomingPickupsInternal)(ImageBase + Finder::FindAFortPlayerPawn_ForceFinishIncomingPickups());
	ForceFinishIncomingPickupsInternal(this);
}

void AFortPlayerPawn::ServerChoosePart(UCustomCharacterPart* ChosenCharacterPart, EFortCustomPartType Part)
{
	static UFunction* Function = nullptr;
	if (!Function)
		Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ServerChoosePart"));

	struct
	{
		EFortCustomPartType Part;
		UCustomCharacterPart* ChosenCharacterPart;
	} Parms{ Part, ChosenCharacterPart };

	ProcessEvent(Function, &Parms);
}