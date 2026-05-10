#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AttributeSet.h"

struct FFortGameplayAttributeData : public FGameplayAttributeData {
public:
	DefineUnrealStruct(FFortGameplayAttributeData);
};

class UFortAttributeSet : public UAttributeSet {
public:
	DefineUnrealClass(UFortAttributeSet);
};