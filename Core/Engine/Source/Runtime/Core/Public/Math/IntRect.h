#pragma once
#include "pch.h"

#include "IntPoint.h"

struct FIntRect
{
	FIntPoint Min;
	FIntPoint Max;

public:
	FIntRect() : Min(0, 0), Max(0, 0) {}
	FIntRect(int32 X0, int32 Y0, int32 X1, int32 Y1) : Min(X0, Y0), Max(X1, Y1) {}
	FIntRect(FIntPoint InMin, FIntPoint InMax) : Min(InMin), Max(InMax) {}

public:
	FORCEINLINE bool operator==(const FIntRect& Other) const { return Min == Other.Min && Max == Other.Max; }
	FORCEINLINE bool operator!=(const FIntRect& Other) const { return Min != Other.Min || Max != Other.Max; }

	FORCEINLINE FIntRect operator+(const FIntPoint& Point) const { return FIntRect(Min + Point, Max + Point); }
	FORCEINLINE FIntRect operator-(const FIntPoint& Point) const { return FIntRect(Min - Point, Max - Point); }

public:
	FORCEINLINE int32 Area() const { return (Max.X - Min.X) * (Max.Y - Min.Y); }
	FORCEINLINE int32 Width() const { return Max.X - Min.X; }
	FORCEINLINE int32 Height() const { return Max.Y - Min.Y; }
	FORCEINLINE FIntPoint Size() const { return FIntPoint(Max.X - Min.X, Max.Y - Min.Y); }

	FORCEINLINE bool Contains(FIntPoint P) const
	{
		return P.X >= Min.X && P.X < Max.X && P.Y >= Min.Y && P.Y < Max.Y;
	}

	FORCEINLINE bool Intersect(const FIntRect& Other) const
	{
		return Other.Min.X < Max.X && Other.Max.X > Min.X && Other.Min.Y < Max.Y && Other.Max.Y > Min.Y;
	}

	FORCEINLINE void Union(const FIntRect& Other)
	{
		Min.X = Min.X < Other.Min.X ? Min.X : Other.Min.X;
		Min.Y = Min.Y < Other.Min.Y ? Min.Y : Other.Min.Y;
		Max.X = Max.X > Other.Max.X ? Max.X : Other.Max.X;
		Max.Y = Max.Y > Other.Max.Y ? Max.Y : Other.Max.Y;
	}
};

static_assert(sizeof(FIntRect) == 0x10, "FIntRect layout broke: two FIntPoints expected (0x10)");
