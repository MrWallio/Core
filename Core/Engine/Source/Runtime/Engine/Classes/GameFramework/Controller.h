#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"

class ACharacter;
class APawn;
class APlayerState;
class FDebugDisplayInfo;
class UDamageType;

class AController : public AActor {
public:
	DefineUnrealClass(AController);
public:
	/** PlayerState containing replicated information about the player using this controller (only exists for players, not NPCs). */
	DefineUProperty(APlayerState*, PlayerState);

	DefineUProperty(APawn*, Pawn);
public:
	void Possess(APawn* InPawn);
};