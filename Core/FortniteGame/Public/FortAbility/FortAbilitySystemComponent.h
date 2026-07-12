#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

class UFortAbilitySet;

class UFortAbilitySystemComponent : public UAbilitySystemComponent {
public:
	DefineUnrealClass(UFortAbilitySystemComponent);
public:
	void GiveAbilitySet(UFortAbilitySet* AbilitySet);

	void EndAllAbilities();

	void EndDBNOAbilities();

	void PrintAllAbilities();

	void EndAllAbilitiesFromAbilitySet(UFortAbilitySet* AbilitySet);

	void EndAbility(UGameplayAbility* Ability);
};

class UFortAbilitySystemComponentAthena : public UFortAbilitySystemComponent {
public:
	DefineUnrealClass(UFortAbilitySystemComponentAthena);
};