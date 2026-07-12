#pragma once
#include "pch.h"

#include "Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FPlane : public FVector
{
public:
	float W;
public:
	FORCEINLINE FPlane() : FVector(), W(0.f) {}

	FORCEINLINE FPlane(float InX, float InY, float InZ, float InW)
		: FVector(InX, InY, InZ), W(InW)
	{
	}

	FORCEINLINE FPlane(FVector InNormal, float InW)
		: FVector(InNormal), W(InW)
	{
	}

	FORCEINLINE FPlane(FVector InBase, const FVector& InNormal)
		: FVector(InNormal), W(InBase | InNormal)
	{
	}

public:
	FORCEINLINE float PlaneDot(const FVector& P) const
	{
		return X * P.X + Y * P.Y + Z * P.Z - W;
	}

	FORCEINLINE FVector GetNormal() const { return FVector(X, Y, Z); }

	FORCEINLINE bool operator==(const FPlane& V) const { return X == V.X && Y == V.Y && Z == V.Z && W == V.W; }
	FORCEINLINE bool operator!=(const FPlane& V) const { return X != V.X || Y != V.Y || Z != V.Z || W != V.W; }

	FORCEINLINE bool Equals(const FPlane& V, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(X - V.X) < Tolerance && FMath::Abs(Y - V.Y) < Tolerance && FMath::Abs(Z - V.Z) < Tolerance && FMath::Abs(W - V.W) < Tolerance;
	}

	FORCEINLINE FPlane Flip() const { return FPlane(-X, -Y, -Z, -W); }
};

static_assert(sizeof(FPlane) == 0x10, "FPlane layout broke: FVector + W expected (0x10)");
