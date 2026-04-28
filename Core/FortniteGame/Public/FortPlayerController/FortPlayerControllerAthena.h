#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

#include "FortPlayerControllerZone.h"

class AFortPlayerPawnAthena;
class AFortBroadcastRemoteClientInfo;
class UFortHero;

class AFortPlayerControllerAthena : public AFortPlayerControllerZone {
public:
	DefineUnrealClass(AFortPlayerControllerAthena);

	DefineUProperty(AFortBroadcastRemoteClientInfo*, BroadcastRemoteClientInfo);

	DefineUProperty(UFortHero*, StrongMyHero);
public:
	static void Hook() {
		Log("Hooked AFortPlayerControllerAthena");
	}
};