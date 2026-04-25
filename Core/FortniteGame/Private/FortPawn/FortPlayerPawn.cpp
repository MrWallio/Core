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

void AFortPlayerPawn::ServerChoosePart(EFortCustomPartType Part, UCustomCharacterPart* ChosenCharacterPart)
{
	if (Finder::FindAFortPlayerPawn_ServerChoosePart()) {
		void (*ServerChoosePartInternal)(AFortPlayerPawn * This, EFortCustomPartType Part, UCustomCharacterPart * ChosenCharacterPart) = decltype(ServerChoosePartInternal)(ImageBase + Finder::FindAFortPlayerPawn_ServerChoosePart());
		ServerChoosePartInternal(this, Part, ChosenCharacterPart);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ServerChoosePart"));

		if (!Func) {
			Log("AFortPlayerPawn::ServerChoosePart: Failed to find function!");
			return;
		}

		struct FortPlayerPawn_ServerChoosePart
		{
		public:
			EFortCustomPartType Part;
			uint8 Pad_1[0x7];
			UCustomCharacterPart* ChosenCharacterPart;
		};

		FortPlayerPawn_ServerChoosePart Parms{};

		Parms.Part = Part;
		Parms.ChosenCharacterPart = ChosenCharacterPart;

		ProcessEvent(Func, &Parms);
	}
}