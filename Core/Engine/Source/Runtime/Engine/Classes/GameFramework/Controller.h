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
	DefineUProperty(APlayerState*, PlayerState);

	DefineUProperty(APawn*, Pawn);
public:
	void Possess(APawn* InPawn);

	APawn* K2_GetPawn() const;

	AActor* GetViewTarget() const;

	void InitPlayerState();
};