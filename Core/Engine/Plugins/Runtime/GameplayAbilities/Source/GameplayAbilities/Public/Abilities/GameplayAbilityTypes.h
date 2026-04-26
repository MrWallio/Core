#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class APlayerController;
class UAbilitySystemComponent;
class UAnimMontage;
class UGameplayAbility;
class UMovementComponent;
class UDataTable;
class UGameplayTask;

struct FGameplayEventData {
public:
	DefineUnrealClass(FGameplayEventData);
};