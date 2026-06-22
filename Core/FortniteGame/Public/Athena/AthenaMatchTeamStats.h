#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FAthenaMatchTeamStats
{
public:
	DefineUnrealStruct(FAthenaMatchTeamStats);

	DefineStructProperty(int32, Place);
	DefineStructProperty(int32, TotalPlayers);
public:
	uint8 Padding[0x10];
};