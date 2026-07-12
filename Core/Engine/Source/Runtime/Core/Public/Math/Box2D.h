#pragma once
#include "pch.h"

#include "Vector2D.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FBox2D
{
public:
	FVector2D Min;
	FVector2D Max;
	uint8 bIsValid;
public:
	FORCEINLINE FBox2D() : Min(0, 0), Max(0, 0), bIsValid(0) {}

	FORCEINLINE FBox2D(const FVector2D& InMin, const FVector2D& InMax)
		: Min(InMin), Max(InMax), bIsValid(1)
	{
	}

public:
	FORCEINLINE FBox2D& operator+=(const FVector2D& Other)
	{
		if (bIsValid)
		{
			Min.X = FMath::Min(Min.X, Other.X);
			Min.Y = FMath::Min(Min.Y, Other.Y);
			Max.X = FMath::Max(Max.X, Other.X);
			Max.Y = FMath::Max(Max.Y, Other.Y);
		}
		else
		{
			Min = Max = Other;
			bIsValid = 1;
		}
		return *this;
	}

	FORCEINLINE FVector2D GetCenter() const { return (Min + Max) * 0.5f; }
	FORCEINLINE FVector2D GetExtent() const { return (Max - Min) * 0.5f; }
	FORCEINLINE FVector2D GetSize() const { return Max - Min; }
	FORCEINLINE float GetArea() const { return (Max.X - Min.X) * (Max.Y - Min.Y); }

	FORCEINLINE bool IsInside(const FVector2D& TestPoint) const
	{
		return TestPoint.X > Min.X && TestPoint.X < Max.X && TestPoint.Y > Min.Y && TestPoint.Y < Max.Y;
	}

	FORCEINLINE bool Intersect(const FBox2D& Other) const
	{
		if (Min.X > Other.Max.X || Other.Min.X > Max.X) return false;
		if (Min.Y > Other.Max.Y || Other.Min.Y > Max.Y) return false;
		return true;
	}
};

static_assert(sizeof(FBox2D) == 0x14, "FBox2D layout broke: two FVector2Ds + bIsValid expected (0x14)");
