#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FGameplayAttributeData {
public:
	DefineUnrealStruct(FGameplayAttributeData);

	FGameplayAttributeData()
		: BaseValue(0.f)
		, CurrentValue(0.f)
	{
	}

	FGameplayAttributeData(float DefaultValue)
		: BaseValue(DefaultValue)
		, CurrentValue(DefaultValue)
	{
	}

public:
	void** VTable;
public:
	float BaseValue;

	float CurrentValue;
};

class UAttributeSet : public UObject {
public:
	DefineUnrealClass(UAttributeSet);
};