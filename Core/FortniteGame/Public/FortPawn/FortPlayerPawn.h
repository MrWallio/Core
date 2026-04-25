#pragma once
#include "pch.h"

#include "FortPawn.h"

class AFortPlayerPawn : public AFortPawn {
public:
	DefineUnrealClass(AFortPlayerPawn);

	DefineUProperty(float, PickupSpeedMultiplier);
public:
	void BeginSkydiving(bool bFromBus);

	void ForceFinishIncomingPickups();

	void ServerChoosePart(UCustomCharacterPart* ChosenCharacterPart, EFortCustomPartType Part);
};