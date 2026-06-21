#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FAthenaLevelInfo {
public:
	DefineUnrealStruct(FAthenaLevelInfo);

	DefineStructProperty(int32, AccountLevel);
	DefineStructProperty(int32, Level);
	DefineStructProperty(int32, MaxLevel);
	DefineStructProperty(int32, LevelXp);
	DefineStructProperty(int32, LevelXpForLevel);
	DefineStructProperty(int32, BookLevel);
	DefineStructProperty(int32, BookMaxLevel);
	DefineStructProperty(int32, BookLevelXp);
	DefineStructProperty(int32, BookLevelXpForLevel);
};