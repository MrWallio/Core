#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "ScalableFloat.h"

enum class EGameplayEffectDurationType : uint8
{
	Instant,
	Infinite,
	HasDuration,
};

enum class EGameplayEffectStackingType : uint8
{
	None,
	AggregateBySource,
	AggregateByTarget,
};

class UGameplayEffect : public UObject {
public:
	DefineUnrealClass(UGameplayEffect);

	DefineUProperty(EGameplayEffectDurationType, DurationPolicy);
	DefineUProperty(FScalableFloat, Period);
	DefineUProperty(FScalableFloat, ChanceToApplyToTarget);
	DefineUProperty(EGameplayEffectStackingType, StackingType);
	DefineUProperty(int32, StackLimitCount);
public:
	FORCEINLINE EGameplayEffectDurationType GetDurationPolicy() const { return DurationPolicy; }
};
