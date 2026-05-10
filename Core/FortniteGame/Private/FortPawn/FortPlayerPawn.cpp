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
	void (*&ForceFinishIncomingPickupsInternal)(AFortPlayerPawn * This) = decltype(ForceFinishIncomingPickupsInternal)(VTable[Finder::FindAFortPlayerPawn_ForceFinishIncomingPickupsVFT()]);
	ForceFinishIncomingPickupsInternal(this);
}

void AFortPlayerPawn::ServerChoosePart(UCustomCharacterPart* ChosenCharacterPart, EFortCustomPartType Part)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ServerChoosePart"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ServerChoosePartInternal)(AFortPlayerPawn*, EFortCustomPartType, UCustomCharacterPart*) = decltype(ServerChoosePartInternal)(VTable[VTableIdx]);
		return ServerChoosePartInternal(this, Part, ChosenCharacterPart);
	}
}

void AFortPlayerPawn::RandomizeCharacter(const FString& GenderString)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("RandomizeCharacter");

	struct FortPlayerPawn_RandomizeCharacter
	{
	public:
		FString GenderString;
	};

	FortPlayerPawn_RandomizeCharacter Parms{};

	Parms.GenderString = std::move(GenderString);

	ProcessEvent(Func, &Parms);
}

void AFortPlayerPawn::ServerReviveFromDBNO(AFortPlayerPawn* This, AController* EventInstigator)
{
	ServerReviveFromDBNOOG(This, EventInstigator);

	Log("ServerReviveFromDBNO Called!");
}