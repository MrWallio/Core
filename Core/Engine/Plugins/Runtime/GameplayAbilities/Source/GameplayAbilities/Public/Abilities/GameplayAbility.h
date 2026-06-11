#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UAbilitySystemComponent;
class UAnimMontage;
class UGameplayAbility;
class UGameplayTask;
class UGameplayTasksComponent;

class UGameplayAbility : public UObject {
public:
	DefineUnrealClass(UGameplayAbility);

	DefineUProperty(uint8, InstancingPolicy);
};