#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FRandomStream
{
public:
	DefineUnrealStruct(FRandomStream);
public:
	mutable int32 InitialSeed;
	mutable int32 Seed;
public:
	FORCEINLINE FRandomStream() : InitialSeed(0), Seed(0) {}

	FORCEINLINE FRandomStream(int32 InSeed) : InitialSeed(InSeed), Seed(InSeed) {}

public:
	FORCEINLINE void Initialize(int32 InSeed)
	{
		InitialSeed = InSeed;
		Seed = InSeed;
	}

	FORCEINLINE void Reset() const { Seed = InitialSeed; }

	FORCEINLINE int32 GetInitialSeed() const { return InitialSeed; }

	FORCEINLINE void MutateSeed() const
	{
		Seed = (Seed * 196314165) + 907633515;
	}

	FORCEINLINE float GetFraction() const
	{
		MutateSeed();

		const float SRandTemp = 1.0f;
		float Result;

		*(int32*)&Result = (*(const int32*)&SRandTemp & 0xff800000) | (Seed & 0x007fffff);

		return FMath::Frac(Result);
	}

	FORCEINLINE uint32 GetUnsignedInt() const
	{
		MutateSeed();
		return *(uint32*)&Seed;
	}

	FORCEINLINE int32 RandRange(int32 Min, int32 Max) const
	{
		const int32 Range = (Max - Min) + 1;
		return Min + RandHelper(Range);
	}

	FORCEINLINE float FRandRange(float InMin, float InMax) const
	{
		return InMin + (InMax - InMin) * GetFraction();
	}

	FORCEINLINE int32 RandHelper(int32 A) const
	{
		return A > 0 ? FMath::Min(FMath::TruncToInt(GetFraction() * (float)A), A - 1) : 0;
	}

	FVector GetUnitVector() const
	{
		FVector Result;
		float L;

		do
		{
			Result.X = GetFraction() * 2.f - 1.f;
			Result.Y = GetFraction() * 2.f - 1.f;
			Result.Z = GetFraction() * 2.f - 1.f;
			L = Result.SizeSquared();
		} while (L > 1.0f || L < KINDA_SMALL_NUMBER);

		return Result.GetUnsafeNormal();
	}
};

static_assert(sizeof(FRandomStream) == 0x8, "FRandomStream layout broke: InitialSeed + Seed expected (0x8)");
