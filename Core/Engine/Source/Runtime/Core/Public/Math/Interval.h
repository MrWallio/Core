#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"

struct FFloatInterval
{
	float Min;
	float Max;

	FFloatInterval() : Min(0.f), Max(0.f) {}
	FFloatInterval(float InMin, float InMax) : Min(InMin), Max(InMax) {}

	FORCEINLINE bool Contains(float Element) const { return Element >= Min && Element <= Max; }
	FORCEINLINE float Size() const { return Max - Min; }
	FORCEINLINE float Interpolate(float Alpha) const { return Min + Alpha * Size(); }
	FORCEINLINE void Include(float X)
	{
		if (X < Min) Min = X;
		if (X > Max) Max = X;
	}
};

struct FInt32Interval
{
	int32 Min;
	int32 Max;

	FInt32Interval() : Min(0), Max(0) {}
	FInt32Interval(int32 InMin, int32 InMax) : Min(InMin), Max(InMax) {}

	FORCEINLINE bool Contains(int32 Element) const { return Element >= Min && Element <= Max; }
	FORCEINLINE int32 Size() const { return Max - Min; }
};

static_assert(sizeof(FFloatInterval) == 0x8, "FFloatInterval layout broke: two floats expected (0x8)");
static_assert(sizeof(FInt32Interval) == 0x8, "FInt32Interval layout broke: two int32 expected (0x8)");
