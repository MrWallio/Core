#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

struct FGameplayEffectApplicationInfo {
public:
	DefineUnrealStruct(FGameplayEffectApplicationInfo);

	DefineStructProperty(TSoftClassPtr<UClass>, GameplayEffect);
	DefineStructProperty(float, Level);
};

struct FGameplayEffectApplicationInfoHard {
public:
	DefineUnrealStruct(FGameplayEffectApplicationInfoHard);

	DefineStructProperty(TSoftClassPtr<UClass>, GameplayEffect);
	DefineStructProperty(float, Level);
};