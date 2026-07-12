#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct alignas(0x10) FVector4
{
public:
	DefineUnrealStruct(FVector4);
public:
	float X;
	float Y;
	float Z;
	float W;
public:
	FORCEINLINE FVector4() : X(0.f), Y(0.f), Z(0.f), W(0.f) {}

	FORCEINLINE FVector4(float InX, float InY, float InZ, float InW = 1.0f)
		: X(InX), Y(InY), Z(InZ), W(InW)
	{
	}

	FORCEINLINE FVector4(const FVector& InVector, float InW = 1.0f)
		: X(InVector.X), Y(InVector.Y), Z(InVector.Z), W(InW)
	{
	}

public:
	FORCEINLINE FVector4 operator+(const FVector4& V) const { return FVector4(X + V.X, Y + V.Y, Z + V.Z, W + V.W); }
	FORCEINLINE FVector4 operator-(const FVector4& V) const { return FVector4(X - V.X, Y - V.Y, Z - V.Z, W - V.W); }
	FORCEINLINE FVector4 operator*(float Scale) const { return FVector4(X * Scale, Y * Scale, Z * Scale, W * Scale); }
	FORCEINLINE FVector4 operator/(float Scale) const { const float RScale = 1.f / Scale; return FVector4(X * RScale, Y * RScale, Z * RScale, W * RScale); }
	FORCEINLINE FVector4 operator*(const FVector4& V) const { return FVector4(X * V.X, Y * V.Y, Z * V.Z, W * V.W); }
	FORCEINLINE FVector4 operator-() const { return FVector4(-X, -Y, -Z, -W); }

	FORCEINLINE FVector4 operator+=(const FVector4& V) { X += V.X; Y += V.Y; Z += V.Z; W += V.W; return *this; }
	FORCEINLINE FVector4 operator-=(const FVector4& V) { X -= V.X; Y -= V.Y; Z -= V.Z; W -= V.W; return *this; }
	FORCEINLINE FVector4 operator*=(float S) { X *= S; Y *= S; Z *= S; W *= S; return *this; }

	FORCEINLINE bool operator==(const FVector4& V) const { return X == V.X && Y == V.Y && Z == V.Z && W == V.W; }
	FORCEINLINE bool operator!=(const FVector4& V) const { return X != V.X || Y != V.Y || Z != V.Z || W != V.W; }

	FORCEINLINE float& operator[](int32 Index) { return (&X)[Index]; }
	FORCEINLINE float operator[](int32 Index) const { return (&X)[Index]; }

public:
	FORCEINLINE static float Dot3(const FVector4& V1, const FVector4& V2) { return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z; }
	FORCEINLINE static float Dot4(const FVector4& V1, const FVector4& V2) { return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z + V1.W * V2.W; }

public:
	FORCEINLINE void Set(float InX, float InY, float InZ, float InW) { X = InX; Y = InY; Z = InZ; W = InW; }

	FORCEINLINE float Size3() const { return FMath::Sqrt(X * X + Y * Y + Z * Z); }
	FORCEINLINE float SizeSquared3() const { return X * X + Y * Y + Z * Z; }
	FORCEINLINE float Size() const { return FMath::Sqrt(X * X + Y * Y + Z * Z + W * W); }
	FORCEINLINE float SizeSquared() const { return X * X + Y * Y + Z * Z + W * W; }

	FORCEINLINE bool Equals(const FVector4& V, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(X - V.X) <= Tolerance && FMath::Abs(Y - V.Y) <= Tolerance && FMath::Abs(Z - V.Z) <= Tolerance && FMath::Abs(W - V.W) <= Tolerance;
	}

	FVector4 GetSafeNormal(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y + Z * Z;
		if (SquareSum > Tolerance)
		{
			const float Scale = FMath::InvSqrt(SquareSum);
			return FVector4(X * Scale, Y * Scale, Z * Scale, 0.0f);
		}
		return FVector4(0.f, 0.f, 0.f, 0.f);
	}

	FORCEINLINE operator FVector() const { return FVector(X, Y, Z); }
};

using FVector4f = FVector4;

static_assert(sizeof(FVector4) == 0x10, "FVector4 layout broke: UE 4.x expects four floats (0x10)");
static_assert(alignof(FVector4) == 0x10, "FVector4 alignment broke: UE aligns FVector4 to 16");
