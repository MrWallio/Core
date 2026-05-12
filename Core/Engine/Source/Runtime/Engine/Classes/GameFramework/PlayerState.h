#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Info.h"

class APlayerState : public AInfo {
public:
	DefineUnrealClass(APlayerState);

	DefineBitfieldUProperty(bOnlySpectator);

	DefineUProperty(uint8, Ping);

	DefineBitfieldUProperty(bIsABot);
};