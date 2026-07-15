#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

struct FVector_NetQuantize : public FVector
{
	FORCEINLINE FVector_NetQuantize() {}
	FORCEINLINE FVector_NetQuantize(float InX, float InY, float InZ) : FVector(InX, InY, InZ) {}
	FORCEINLINE FVector_NetQuantize(const FVector& InVec) : FVector(InVec) {}
};

struct FVector_NetQuantize10 : public FVector
{
	FORCEINLINE FVector_NetQuantize10() {}
	FORCEINLINE FVector_NetQuantize10(float InX, float InY, float InZ) : FVector(InX, InY, InZ) {}
	FORCEINLINE FVector_NetQuantize10(const FVector& InVec) : FVector(InVec) {}
};

struct FVector_NetQuantize100 : public FVector
{
	FORCEINLINE FVector_NetQuantize100() {}
	FORCEINLINE FVector_NetQuantize100(float InX, float InY, float InZ) : FVector(InX, InY, InZ) {}
	FORCEINLINE FVector_NetQuantize100(const FVector& InVec) : FVector(InVec) {}
};

struct FVector_NetQuantizeNormal : public FVector
{
	FORCEINLINE FVector_NetQuantizeNormal() {}
	FORCEINLINE FVector_NetQuantizeNormal(float InX, float InY, float InZ) : FVector(InX, InY, InZ) {}
	FORCEINLINE FVector_NetQuantizeNormal(const FVector& InVec) : FVector(InVec) {}
};

static_assert(sizeof(FVector_NetQuantize) == sizeof(FVector), "FVector_NetQuantize adds no members; quantization only affects net serialization");
static_assert(sizeof(FVector_NetQuantizeNormal) == sizeof(FVector), "FVector_NetQuantizeNormal adds no members; quantization only affects net serialization");