#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"

struct FGameplayEffectContext {
public:
	DefineUnrealStruct(FGameplayEffectContext);
};

struct FGameplayEffectContextHandle {
public:
	DefineUnrealStruct(FGameplayEffectContextHandle);
public:
	TSharedPtr<FGameplayEffectContext> Data;
};