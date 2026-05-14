#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AttributeSet.h"

struct FFortGameplayAttributeData : public FGameplayAttributeData {
public:
	DefineUnrealStruct(FFortGameplayAttributeData);

	DefineStructProperty(float, Minimum);
	DefineStructProperty(float, Maximum);
};

class UFortAttributeSet : public UAttributeSet {
public:
	DefineUnrealClass(UFortAttributeSet);
};