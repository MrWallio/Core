#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UAbilitySystemComponent;

struct FActiveGameplayEffectHandle {
public:
	DefineUnrealStruct(FActiveGameplayEffectHandle);

	FActiveGameplayEffectHandle()
		: Handle(INDEX_NONE),
		bPassedFiltersAndWasExecuted(false)
	{
	}

	FActiveGameplayEffectHandle(int32 InHandle)
		: Handle(InHandle),
		bPassedFiltersAndWasExecuted(true)
	{
	}

	bool IsValid() const
	{
		return Handle != INDEX_NONE;
	}

	bool WasSuccessfullyApplied() const
	{
		return bPassedFiltersAndWasExecuted;
	}

	bool operator==(const FActiveGameplayEffectHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FActiveGameplayEffectHandle& Other) const
	{
		return Handle != Other.Handle;
	}

	friend uint32 GetTypeHash(const FActiveGameplayEffectHandle& InHandle)
	{
		return InHandle.Handle;
	}

	void Invalidate()
	{
		Handle = INDEX_NONE;
	}

public:
	int32 Handle;

	bool bPassedFiltersAndWasExecuted;
};