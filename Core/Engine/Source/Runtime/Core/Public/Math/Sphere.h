#pragma once
#include "pch.h"

#include "Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class FSphere
{
public:
	FVector Center;
	float W;
public:
	FORCEINLINE FSphere() : Center(0, 0, 0), W(0) {}

	FORCEINLINE FSphere(const FVector& InV, float InW)
		: Center(InV), W(InW)
	{
	}

public:
	FORCEINLINE bool operator==(const FSphere& Other) const { return Center == Other.Center && W == Other.W; }

	FORCEINLINE bool IsInside(const FVector& In, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return (Center - In).SizeSquared() <= FMath::Square(W + Tolerance);
	}

	FORCEINLINE bool IsInside(const FSphere& Other, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		if (W - Other.W < -Tolerance)
		{
			return false;
		}
		return (Center - Other.Center).SizeSquared() <= FMath::Square(W + Tolerance - Other.W);
	}

	FORCEINLINE bool Intersects(const FSphere& Other, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return (Center - Other.Center).SizeSquared() <= FMath::Square(FMath::Max(0.f, Other.W + W + Tolerance));
	}

	FORCEINLINE FSphere ExpandBy(float ExpandAmount) const
	{
		return FSphere(Center, W + ExpandAmount);
	}
};

static_assert(sizeof(FSphere) == 0x10, "FSphere layout broke: FVector + W expected (0x10)");
