#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpecHandle.h"

class UAbilitySystemComponent;
class UAnimMontage;
class UGameplayAbility;
class UGameplayTask;
class UGameplayTasksComponent;
class FGameplayAbilityActorInfo;
class FGameplayTagContainer;

class UGameplayAbility : public UObject {
public:
	DefineUnrealClass(UGameplayAbility);

	DefineUProperty(uint8, InstancingPolicy);
	DefineUProperty(uint8, NetSecurityPolicy);

public:
	static inline bool (*CanActivateAbilityOG)(UGameplayAbility* This, FGameplayAbilitySpecHandle Handle, FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* SourceTags, FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags);
	static bool CanActivateAbility(UGameplayAbility* This, FGameplayAbilitySpecHandle Handle, FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* SourceTags, FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags);

	static void Hook();
};