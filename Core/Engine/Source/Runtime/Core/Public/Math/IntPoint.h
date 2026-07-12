#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeHash.h"

struct FIntPoint
{
	int32 X;
	int32 Y;

public:
	static const FIntPoint ZeroValue;
	static const FIntPoint NoneValue;

public:
	FIntPoint() : X(0), Y(0) {}
	FIntPoint(int32 InX, int32 InY) : X(InX), Y(InY) {}
	explicit FIntPoint(int32 InXY) : X(InXY), Y(InXY) {}

public:
	FORCEINLINE FIntPoint operator+(const FIntPoint& Other) const { return FIntPoint(X + Other.X, Y + Other.Y); }
	FORCEINLINE FIntPoint operator-(const FIntPoint& Other) const { return FIntPoint(X - Other.X, Y - Other.Y); }
	FORCEINLINE FIntPoint operator*(int32 Scale) const { return FIntPoint(X * Scale, Y * Scale); }
	FORCEINLINE FIntPoint operator/(int32 Divisor) const { return FIntPoint(X / Divisor, Y / Divisor); }

	FORCEINLINE FIntPoint& operator+=(const FIntPoint& Other) { X += Other.X; Y += Other.Y; return *this; }
	FORCEINLINE FIntPoint& operator-=(const FIntPoint& Other) { X -= Other.X; Y -= Other.Y; return *this; }

	FORCEINLINE bool operator==(const FIntPoint& Other) const { return X == Other.X && Y == Other.Y; }
	FORCEINLINE bool operator!=(const FIntPoint& Other) const { return X != Other.X || Y != Other.Y; }

	FORCEINLINE int32& operator[](int32 Index) { return (&X)[Index]; }
	FORCEINLINE int32 operator[](int32 Index) const { return (&X)[Index]; }

public:
	FORCEINLINE int32 GetMax() const { return X > Y ? X : Y; }
	FORCEINLINE int32 GetMin() const { return X < Y ? X : Y; }
	FORCEINLINE int32 Size() const { return (int32)sqrtf((float)(X * (int64_t)X + Y * (int64_t)Y)); }
	FORCEINLINE int32 SizeSquared() const { return X * X + Y * Y; }

	friend uint32 GetTypeHash(const FIntPoint& InPoint)
	{
		return HashCombine((uint32)InPoint.X, (uint32)InPoint.Y);
	}
};

inline const FIntPoint FIntPoint::ZeroValue(0, 0);
inline const FIntPoint FIntPoint::NoneValue(-1, -1);

static_assert(sizeof(FIntPoint) == 0x8, "FIntPoint layout broke: two int32 expected (0x8)");
