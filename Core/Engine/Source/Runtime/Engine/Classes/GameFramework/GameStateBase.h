#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Info.h"

class APlayerState;
class ASpectatorPawn;

class AGameModeBase;
class ASpectatorPawn;
class APlayerState;
class AController;

class AGameStateBase : public AInfo {
public:
	DefineUnrealClass(AGameStateBase);
};