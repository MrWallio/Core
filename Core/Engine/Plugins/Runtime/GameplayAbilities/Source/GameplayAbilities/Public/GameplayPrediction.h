#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UAbilitySystemComponent;
class UPackageMap;

struct FPredictionKey {
public:
	DefineUnrealStructPath("ScriptStruct /Script/GameplayAbilities.PredictionKey");

	DefineStructProperty(int16, Current);
	DefineStructProperty(int16, Base);
	DefineStructProperty(UPackageMap*, PredictiveConnection);
	DefineStructProperty(bool, bIsStale);
	DefineStructProperty(bool, bIsServerInitiated);

public:
	uint8 Padding[0x18];
};