#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FVector2D
{
public:
	DefineUnrealStruct(FVector2D);
public:
	float X;
	float Y;
public:
	static const FVector2D ZeroVector;
	static const FVector2D UnitVector;
public:
	FORCEINLINE FVector2D() : X(0.f), Y(0.f) {}

	explicit FORCEINLINE FVector2D(float InF) : X(InF), Y(InF) {}

	FORCEINLINE FVector2D(float InX, float InY) : X(InX), Y(InY) {}

public:
	FORCEINLINE FVector2D operator+(const FVector2D& V) const { return FVector2D(X + V.X, Y + V.Y); }
	FORCEINLINE FVector2D operator-(const FVector2D& V) const { return FVector2D(X - V.X, Y - V.Y); }
	FORCEINLINE FVector2D operator*(float Scale) const { return FVector2D(X * Scale, Y * Scale); }
	FORCEINLINE FVector2D operator/(float Scale) const { const float RScale = 1.f / Scale; return FVector2D(X * RScale, Y * RScale); }
	FORCEINLINE FVector2D operator+(float A) const { return FVector2D(X + A, Y + A); }
	FORCEINLINE FVector2D operator-(float A) const { return FVector2D(X - A, Y - A); }
	FORCEINLINE FVector2D operator*(const FVector2D& V) const { return FVector2D(X * V.X, Y * V.Y); }
	FORCEINLINE FVector2D operator/(const FVector2D& V) const { return FVector2D(X / V.X, Y / V.Y); }
	FORCEINLINE FVector2D operator-() const { return FVector2D(-X, -Y); }

	FORCEINLINE float operator|(const FVector2D& V) const { return X * V.X + Y * V.Y; }
	FORCEINLINE float operator^(const FVector2D& V) const { return X * V.Y - Y * V.X; }

	FORCEINLINE FVector2D operator+=(const FVector2D& V) { X += V.X; Y += V.Y; return *this; }
	FORCEINLINE FVector2D operator-=(const FVector2D& V) { X -= V.X; Y -= V.Y; return *this; }
	FORCEINLINE FVector2D operator*=(float Scale) { X *= Scale; Y *= Scale; return *this; }
	FORCEINLINE FVector2D operator/=(float V) { const float RV = 1.f / V; X *= RV; Y *= RV; return *this; }

	FORCEINLINE bool operator==(const FVector2D& V) const { return X == V.X && Y == V.Y; }
	FORCEINLINE bool operator!=(const FVector2D& V) const { return X != V.X || Y != V.Y; }

	FORCEINLINE float& operator[](int32 Index) { return (&X)[Index]; }
	FORCEINLINE float operator[](int32 Index) const { return (&X)[Index]; }

public:
	FORCEINLINE static float DotProduct(const FVector2D& A, const FVector2D& B) { return A | B; }
	FORCEINLINE static float CrossProduct(const FVector2D& A, const FVector2D& B) { return A ^ B; }

	FORCEINLINE static float DistSquared(const FVector2D& V1, const FVector2D& V2)
	{
		return FMath::Square(V2.X - V1.X) + FMath::Square(V2.Y - V1.Y);
	}

	FORCEINLINE static float Distance(const FVector2D& V1, const FVector2D& V2) { return FMath::Sqrt(DistSquared(V1, V2)); }

public:
	FORCEINLINE void Set(float InX, float InY) { X = InX; Y = InY; }

	FORCEINLINE bool Equals(const FVector2D& V, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(X - V.X) <= Tolerance && FMath::Abs(Y - V.Y) <= Tolerance;
	}

	FORCEINLINE float Size() const { return FMath::Sqrt(X * X + Y * Y); }
	FORCEINLINE float SizeSquared() const { return X * X + Y * Y; }

	FORCEINLINE float GetMax() const { return FMath::Max(X, Y); }
	FORCEINLINE float GetAbsMax() const { return FMath::Max(FMath::Abs(X), FMath::Abs(Y)); }
	FORCEINLINE float GetMin() const { return FMath::Min(X, Y); }

	FORCEINLINE FVector2D GetAbs() const { return FVector2D(FMath::Abs(X), FMath::Abs(Y)); }

	FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(X) <= Tolerance && FMath::Abs(Y) <= Tolerance;
	}

	FORCEINLINE bool IsZero() const { return X == 0.f && Y == 0.f; }

	FORCEINLINE FVector2D GetRotated(float AngleDeg) const
	{
		float S, C;
		FMath::SinCos(&S, &C, FMath::DegreesToRadians(AngleDeg));
		return FVector2D(C * X - S * Y, S * X + C * Y);
	}

	FVector2D GetSafeNormal(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y;
		if (SquareSum > Tolerance)
		{
			const float Scale = FMath::InvSqrt(SquareSum);
			return FVector2D(X * Scale, Y * Scale);
		}
		return FVector2D(0.f, 0.f);
	}

	void Normalize(float Tolerance = SMALL_NUMBER)
	{
		const float SquareSum = X * X + Y * Y;
		if (SquareSum > Tolerance)
		{
			const float Scale = FMath::InvSqrt(SquareSum);
			X *= Scale;
			Y *= Scale;
			return;
		}
		X = 0.0f;
		Y = 0.0f;
	}

	FVector2D ClampAxes(float MinAxisVal, float MaxAxisVal) const
	{
		return FVector2D(FMath::Clamp(X, MinAxisVal, MaxAxisVal), FMath::Clamp(Y, MinAxisVal, MaxAxisVal));
	}

	std::string FormatToString() const {
		return std::format("X={}, Y={}", X, Y);
	}
};

inline const FVector2D FVector2D::ZeroVector(0.f, 0.f);
inline const FVector2D FVector2D::UnitVector(1.f, 1.f);

FORCEINLINE FVector2D operator*(float Scale, const FVector2D& V)
{
	return V * Scale;
}

using FVector2f = FVector2D;

struct FVector2d
{
	double X;
	double Y;

	FVector2d() : X(0), Y(0) {}
	FVector2d(double InX, double InY) : X(InX), Y(InY) {}
	explicit FVector2d(const FVector2D& V) : X(V.X), Y(V.Y) {}

	FVector2D ToFloat() const { return FVector2D((float)X, (float)Y); }
};

static_assert(sizeof(FVector2D) == 0x8, "FVector2D layout broke: UE 4.x expects two floats (0x8)");
static_assert(sizeof(FVector2d) == 0x10, "FVector2d layout broke: UE 5.x expects two doubles (0x10)");
