#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UAbilitySystemComponent;

struct FPredictionKey {
public:
	DefineUnrealStruct(FPredictionKey);

	DefineStructProperty(int16, Current);
};