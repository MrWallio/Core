#pragma once
#include "pch.h"

#include "Vector.h"
#include "Vector4.h"
#include "Plane.h"
#include "Rotator.h"
#include "Axis.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct alignas(0x10) FMatrix
{
public:
	DefineUnrealStruct(FMatrix);
public:
	float M[4][4];
public:
	static const FMatrix Identity;
public:
	FORCEINLINE FMatrix()
	{
		std::memset(M, 0, sizeof(M));
	}

	FORCEINLINE FMatrix(const FPlane& InX, const FPlane& InY, const FPlane& InZ, const FPlane& InW)
	{
		M[0][0] = InX.X; M[0][1] = InX.Y; M[0][2] = InX.Z; M[0][3] = InX.W;
		M[1][0] = InY.X; M[1][1] = InY.Y; M[1][2] = InY.Z; M[1][3] = InY.W;
		M[2][0] = InZ.X; M[2][1] = InZ.Y; M[2][2] = InZ.Z; M[2][3] = InZ.W;
		M[3][0] = InW.X; M[3][1] = InW.Y; M[3][2] = InW.Z; M[3][3] = InW.W;
	}

	FORCEINLINE FMatrix(const FVector& InX, const FVector& InY, const FVector& InZ, const FVector& InW)
	{
		M[0][0] = InX.X; M[0][1] = InX.Y; M[0][2] = InX.Z; M[0][3] = 0.0f;
		M[1][0] = InY.X; M[1][1] = InY.Y; M[1][2] = InY.Z; M[1][3] = 0.0f;
		M[2][0] = InZ.X; M[2][1] = InZ.Y; M[2][2] = InZ.Z; M[2][3] = 0.0f;
		M[3][0] = InW.X; M[3][1] = InW.Y; M[3][2] = InW.Z; M[3][3] = 1.0f;
	}

public:
	FORCEINLINE void SetIdentity()
	{
		M[0][0] = 1; M[0][1] = 0; M[0][2] = 0; M[0][3] = 0;
		M[1][0] = 0; M[1][1] = 1; M[1][2] = 0; M[1][3] = 0;
		M[2][0] = 0; M[2][1] = 0; M[2][2] = 1; M[2][3] = 0;
		M[3][0] = 0; M[3][1] = 0; M[3][2] = 0; M[3][3] = 1;
	}

	FORCEINLINE FMatrix operator*(const FMatrix& Other) const
	{
		FMatrix Result;
		for (int32 i = 0; i < 4; ++i)
		{
			for (int32 j = 0; j < 4; ++j)
			{
				Result.M[i][j] =
					M[i][0] * Other.M[0][j] +
					M[i][1] * Other.M[1][j] +
					M[i][2] * Other.M[2][j] +
					M[i][3] * Other.M[3][j];
			}
		}
		return Result;
	}

	FORCEINLINE void operator*=(const FMatrix& Other) { *this = *this * Other; }

	FORCEINLINE bool operator==(const FMatrix& Other) const
	{
		return std::memcmp(M, Other.M, sizeof(M)) == 0;
	}

	FORCEINLINE FVector4 TransformFVector4(const FVector4& P) const
	{
		return FVector4(
			P.X * M[0][0] + P.Y * M[1][0] + P.Z * M[2][0] + P.W * M[3][0],
			P.X * M[0][1] + P.Y * M[1][1] + P.Z * M[2][1] + P.W * M[3][1],
			P.X * M[0][2] + P.Y * M[1][2] + P.Z * M[2][2] + P.W * M[3][2],
			P.X * M[0][3] + P.Y * M[1][3] + P.Z * M[2][3] + P.W * M[3][3]);
	}

	FORCEINLINE FVector4 TransformPosition(const FVector& V) const
	{
		return TransformFVector4(FVector4(V.X, V.Y, V.Z, 1.0f));
	}

	FORCEINLINE FVector4 TransformVector(const FVector& V) const
	{
		return TransformFVector4(FVector4(V.X, V.Y, V.Z, 0.0f));
	}

	FORCEINLINE FVector GetScaledAxis(EAxis::Type Axis) const
	{
		switch (Axis)
		{
		case EAxis::X: return FVector(M[0][0], M[0][1], M[0][2]);
		case EAxis::Y: return FVector(M[1][0], M[1][1], M[1][2]);
		case EAxis::Z: return FVector(M[2][0], M[2][1], M[2][2]);
		default: return FVector(0.f, 0.f, 0.f);
		}
	}

	FORCEINLINE FVector GetOrigin() const { return FVector(M[3][0], M[3][1], M[3][2]); }

	float Determinant() const
	{
		return
			M[0][0] * (
				M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
				M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])) -
			M[1][0] * (
				M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])) +
			M[2][0] * (
				M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
				M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])) -
			M[3][0] * (
				M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
				M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
				M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2]));
	}

	FMatrix GetTransposed() const
	{
		FMatrix Result;
		for (int32 i = 0; i < 4; ++i)
		{
			for (int32 j = 0; j < 4; ++j)
			{
				Result.M[i][j] = M[j][i];
			}
		}
		return Result;
	}

	FRotator Rotator() const;
	FVector GetScaleVector() const
	{
		FVector Scale3D(1, 1, 1);
		for (int32 i = 0; i < 3; ++i)
		{
			const float SquareSum = M[i][0] * M[i][0] + M[i][1] * M[i][1] + M[i][2] * M[i][2];
			Scale3D[i] = FMath::Sqrt(SquareSum);
		}
		return Scale3D;
	}
};

inline const FMatrix FMatrix::Identity(
	FPlane(1.f, 0.f, 0.f, 0.f),
	FPlane(0.f, 1.f, 0.f, 0.f),
	FPlane(0.f, 0.f, 1.f, 0.f),
	FPlane(0.f, 0.f, 0.f, 1.f));

static_assert(sizeof(FMatrix) == 0x40, "FMatrix layout broke: 4x4 floats expected (0x40)");
static_assert(alignof(FMatrix) == 0x10, "FMatrix alignment broke: UE aligns matrices to 16");
