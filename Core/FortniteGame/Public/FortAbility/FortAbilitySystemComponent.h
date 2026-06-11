#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

class UFortAbilitySystemComponent : public UAbilitySystemComponent {
public:
	DefineUnrealClass(UFortAbilitySystemComponent);
};

class UFortAbilitySystemComponentAthena : public UFortAbilitySystemComponent {
public:
	DefineUnrealClass(UFortAbilitySystemComponentAthena);
};