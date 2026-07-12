#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

struct FIntVector
{
	int32 X;

	int32 Y;

	int32 Z;

public:
	static const FIntVector ZeroValue;
	static const FIntVector NoneValue;

public:
	FIntVector() : X(0), Y(0), Z(0) {}
	FIntVector(int32 InX, int32 InY, int32 InZ) : X(InX), Y(InY), Z(InZ) {}
	explicit FIntVector(int32 InValue) : X(InValue), Y(InValue), Z(InValue) {}

public:
	FORCEINLINE FIntVector operator+(const FIntVector& Other) const { return FIntVector(X + Other.X, Y + Other.Y, Z + Other.Z); }
	FORCEINLINE FIntVector operator-(const FIntVector& Other) const { return FIntVector(X - Other.X, Y - Other.Y, Z - Other.Z); }
	FORCEINLINE FIntVector operator*(int32 Scale) const { return FIntVector(X * Scale, Y * Scale, Z * Scale); }
	FORCEINLINE FIntVector operator/(int32 Divisor) const { return FIntVector(X / Divisor, Y / Divisor, Z / Divisor); }

	FORCEINLINE FIntVector& operator+=(const FIntVector& Other) { X += Other.X; Y += Other.Y; Z += Other.Z; return *this; }
	FORCEINLINE FIntVector& operator-=(const FIntVector& Other) { X -= Other.X; Y -= Other.Y; Z -= Other.Z; return *this; }

	FORCEINLINE bool operator==(const FIntVector& Other) const { return X == Other.X && Y == Other.Y && Z == Other.Z; }
	FORCEINLINE bool operator!=(const FIntVector& Other) const { return X != Other.X || Y != Other.Y || Z != Other.Z; }

	FORCEINLINE int32& operator[](int32 Index) { return (&X)[Index]; }
	FORCEINLINE int32 operator[](int32 Index) const { return (&X)[Index]; }

public:
	FORCEINLINE bool IsZero() const { return X == 0 && Y == 0 && Z == 0; }

	FORCEINLINE int32 GetMax() const { return X > Y ? (X > Z ? X : Z) : (Y > Z ? Y : Z); }
	FORCEINLINE int32 GetMin() const { return X < Y ? (X < Z ? X : Z) : (Y < Z ? Y : Z); }

	friend uint32 GetTypeHash(const FIntVector& Vector)
	{
		return (uint32)Vector.X + ((uint32)Vector.Y * 23) + ((uint32)Vector.Z * 619);
	}
};

inline const FIntVector FIntVector::ZeroValue(0, 0, 0);
inline const FIntVector FIntVector::NoneValue(-1, -1, -1);

static_assert(sizeof(FIntVector) == 0xC, "FIntVector layout broke: three int32 expected (0xC)");
