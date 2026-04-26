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
};