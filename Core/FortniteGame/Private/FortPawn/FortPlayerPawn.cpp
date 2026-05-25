#include "pch.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawn.h"

#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

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

void AFortPlayerPawn::ServerHandlePickup(AFortPlayerPawn* This, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound) {
	if (Version::Fortnite_Version <= 1.81) {
		return ServerHandlePickupOG(This, Pickup, InFlyTime, InStartDirection, bPlayPickupSound);
	}

	if (!This || !Pickup) {
		Log("ServerHandlePickup: This or Pickup is null!");
		return;
	}
	if (Pickup->bPickedUp) {
		Log("ServerHandlePickup: Pickup is already picked up!");
		return;
	}

	Pickup->SetPickupTarget(This, InFlyTime / This->PickupSpeedMultiplier, InStartDirection, bPlayPickupSound);
}