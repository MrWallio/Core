#pragma once
#include "pch.h"

#include "Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FTwoVectors
{
public:
	DefineUnrealStruct(FTwoVectors);
public:
	FVector v1;
	FVector v2;
public:
	FORCEINLINE FTwoVectors() : v1(0, 0, 0), v2(0, 0, 0) {}

	FORCEINLINE FTwoVectors(FVector In1, FVector In2)
		: v1(In1), v2(In2)
	{
	}

public:
	FORCEINLINE FTwoVectors operator+(const FTwoVectors& V) const { return FTwoVectors(v1 + V.v1, v2 + V.v2); }
	FORCEINLINE FTwoVectors operator-(const FTwoVectors& V) const { return FTwoVectors(v1 - V.v1, v2 - V.v2); }
	FORCEINLINE FTwoVectors operator*(float Scale) const { return FTwoVectors(v1 * Scale, v2 * Scale); }

	FORCEINLINE bool operator==(const FTwoVectors& V) const { return v1 == V.v1 && v2 == V.v2; }
	FORCEINLINE bool operator!=(const FTwoVectors& V) const { return v1 != V.v1 || v2 != V.v2; }

	FORCEINLINE FVector& operator[](int32 Index) { return Index == 0 ? v1 : v2; }
};

static_assert(sizeof(FTwoVectors) == 0x18, "FTwoVectors layout broke: two FVectors expected (0x18)");
