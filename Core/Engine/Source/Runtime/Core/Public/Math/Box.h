#pragma once
#include "pch.h"

#include "Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FBox
{
public:
	FVector Min;
	FVector Max;
	uint8 IsValid;
public:
	FORCEINLINE FBox() : Min(0, 0, 0), Max(0, 0, 0), IsValid(0) {}

	FORCEINLINE FBox(const FVector& InMin, const FVector& InMax)
		: Min(InMin), Max(InMax), IsValid(1)
	{
	}

public:
	FORCEINLINE bool operator==(const FBox& Other) const { return Min == Other.Min && Max == Other.Max; }

	FORCEINLINE FBox operator+(const FVector& Other) const
	{
		FBox Result(*this);
		return Result += Other;
	}

	FORCEINLINE FBox& operator+=(const FVector& Other)
	{
		if (IsValid)
		{
			Min = Min.ComponentMin(Other);
			Max = Max.ComponentMax(Other);
		}
		else
		{
			Min = Max = Other;
			IsValid = 1;
		}
		return *this;
	}

	FORCEINLINE FBox operator+(const FBox& Other) const
	{
		FBox Result(*this);
		return Result += Other;
	}

	FORCEINLINE FBox& operator+=(const FBox& Other)
	{
		if (IsValid && Other.IsValid)
		{
			Min = Min.ComponentMin(Other.Min);
			Max = Max.ComponentMax(Other.Max);
		}
		else if (Other.IsValid)
		{
			*this = Other;
		}
		return *this;
	}

public:
	FORCEINLINE FVector GetCenter() const { return (Min + Max) * 0.5f; }
	FORCEINLINE FVector GetExtent() const { return (Max - Min) * 0.5f; }
	FORCEINLINE FVector GetSize() const { return Max - Min; }
	FORCEINLINE float GetVolume() const { return (Max.X - Min.X) * (Max.Y - Min.Y) * (Max.Z - Min.Z); }

	FORCEINLINE void GetCenterAndExtents(FVector& Center, FVector& Extents) const
	{
		Extents = GetExtent();
		Center = Min + Extents;
	}

	FORCEINLINE bool IsInside(const FVector& In) const
	{
		return In.X > Min.X && In.X < Max.X
			&& In.Y > Min.Y && In.Y < Max.Y
			&& In.Z > Min.Z && In.Z < Max.Z;
	}

	FORCEINLINE bool IsInsideOrOn(const FVector& In) const
	{
		return In.X >= Min.X && In.X <= Max.X
			&& In.Y >= Min.Y && In.Y <= Max.Y
			&& In.Z >= Min.Z && In.Z <= Max.Z;
	}

	FORCEINLINE bool Intersect(const FBox& Other) const
	{
		if (Min.X > Other.Max.X || Other.Min.X > Max.X) return false;
		if (Min.Y > Other.Max.Y || Other.Min.Y > Max.Y) return false;
		if (Min.Z > Other.Max.Z || Other.Min.Z > Max.Z) return false;
		return true;
	}

	FORCEINLINE FBox ExpandBy(float W) const
	{
		return FBox(Min - FVector(W, W, W), Max + FVector(W, W, W));
	}

	FORCEINLINE float ComputeSquaredDistanceToPoint(const FVector& Point) const
	{
		float DistSquared = 0.f;
		for (int32 i = 0; i < 3; ++i)
		{
			if (Point[i] < Min[i]) DistSquared += FMath::Square(Point[i] - Min[i]);
			else if (Point[i] > Max[i]) DistSquared += FMath::Square(Point[i] - Max[i]);
		}
		return DistSquared;
	}
};

static_assert(sizeof(FBox) == 0x1C, "FBox layout broke: two FVectors + IsValid byte expected (0x1C)");
