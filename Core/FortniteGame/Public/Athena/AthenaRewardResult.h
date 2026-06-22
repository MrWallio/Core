#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FAthenaRewardResult {
public:
	DefineUnrealStruct(FAthenaRewardResult);

	DefineStructProperty(int32, LevelsGained);
public:
	uint8 Padding[0x50];
};