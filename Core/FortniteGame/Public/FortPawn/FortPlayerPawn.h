#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"

#include "FortPawn.h"

class UFortHero;

class AFortPlayerPawn : public AFortPawn {
public:
	DefineUnrealClass(AFortPlayerPawn);

	DefineUProperty(float, PickupSpeedMultiplier);

	DefineUProperty(TWeakObjectPtr<UFortHero>, Hero);
public:
	void BeginSkydiving(bool bFromBus);

	void ForceFinishIncomingPickups();

	void ServerChoosePart(UCustomCharacterPart* ChosenCharacterPart, EFortCustomPartType Part);

	void RandomizeCharacter(const FString& GenderString);

	static inline void (*ServerReviveFromDBNOOG)(AFortPlayerPawn* This, AController* EventInstigator);
	static void ServerReviveFromDBNO(AFortPlayerPawn* This, AController* EventInstigator);

	static inline void (*ServerHandlePickupOG)(AFortPlayerPawn* This, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound);
	static void ServerHandlePickup(AFortPlayerPawn* This, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound);

	static void Hook() {
		HookEveryVTable(
			AFortPlayerPawn::StaticClass(),
			AFortPlayerPawn::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerPawn.ServerReviveFromDBNO"),
			ServerReviveFromDBNO,
			(LPVOID*)&ServerReviveFromDBNOOG
		);

		HookEveryVTable(
			AFortPlayerPawn::StaticClass(),
			AFortPlayerPawn::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerPawn.ServerHandlePickup"),
			ServerHandlePickup,
			(LPVOID*)&ServerHandlePickupOG
		);
		
		Log("AFortPlayerPawn Hooked!");
	}
};