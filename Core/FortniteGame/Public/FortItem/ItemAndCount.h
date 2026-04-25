#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AFortPlayerController;
class UFortItemDefinition;
class UFortAccountItemDefinition;

struct FFortItemEntry;

struct FItemAndCount {
public:
	DefineUnrealStruct(FItemAndCount);
public:
	DefineStructProperty(int32, Count);

	DefineStructProperty(UFortItemDefinition*, Item);
};