#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UNavigationSystemConfig;

class UAthenaAISettings : public UObject {
public:
	DefineUnrealClass(UAthenaAISettings);

	DefineUProperty(float, MinHearingRange);
	DefineUProperty(float, MaxHearingRange);
	DefineUProperty(float, DeAggroRange);
	DefineUProperty(UNavigationSystemConfig*, NavigationSystemConfig);
};
