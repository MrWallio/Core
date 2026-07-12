#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Axis.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FRotator;
struct FQuat;

struct FVector
{
public:
	DefineUnrealStruct(FVector);
public:
	float X;
	float Y;
	float Z;
public:
	static const FVector ZeroVector;
	static const FVector OneVector;
	static const FVector UpVector;
	static const FVector DownVector;
	static const FVector ForwardVector;
	static const FVector BackwardVector;
	static const FVector RightVector;
	static const FVector LeftVector;
public:
	FORCEINLINE FVector();

	explicit FORCEINLINE FVector(float InF);

	FORCEINLINE FVector(float InX, float InY, float InZ);
public:
	FORCEINLINE float operator|(const FVector& V) const;

	FORCEINLINE FVector operator^(const FVector& V) const
	{
		return FVector(
			Y * V.Z - Z * V.Y,
			Z * V.X - X * V.Z,
			X * V.Y - Y * V.X);
	}

	FORCEINLINE FVector operator+(const FVector& V) const;

	FORCEINLINE FVector operator-(const FVector& V) const;

	FORCEINLINE FVector operator+(float Bias) const;

	FORCEINLINE FVector operator-(float Bias) const { return FVector(X - Bias, Y - Bias, Z - Bias); }

	FORCEINLINE FVector operator*(float Scale) const;

	FORCEINLINE FVector operator/(float Scale) const
	{
		const float RScale = 1.f / Scale;
		return FVector(X * RScale, Y * RScale, Z * RScale);
	}

	FORCEINLINE FVector operator*(const FVector& V) const;

	FORCEINLINE FVector operator/(const FVector& V) const { return FVector(X / V.X, Y / V.Y, Z / V.Z); }

	FORCEINLINE FVector operator-() const { return FVector(-X, -Y, -Z); }

	FORCEINLINE FVector operator+=(const FVector& V);

	FORCEINLINE FVector operator-=(const FVector& V)
	{
		X -= V.X; Y -= V.Y; Z -= V.Z;
		return *this;
	}

	FORCEINLINE FVector operator*=(float Scale)
	{
		X *= Scale; Y *= Scale; Z *= Scale;
		return *this;
	}

	FORCEINLINE FVector operator/=(float V)
	{
		const float RV = 1.f / V;
		X *= RV; Y *= RV; Z *= RV;
		return *this;
	}

	FORCEINLINE FVector operator*=(const FVector& V)
	{
		X *= V.X; Y *= V.Y; Z *= V.Z;
		return *this;
	}

	FORCEINLINE FVector operator/=(const FVector& V)
	{
		X /= V.X; Y /= V.Y; Z /= V.Z;
		return *this;
	}

	FORCEINLINE bool operator==(const FVector& V) const { return X == V.X && Y == V.Y && Z == V.Z; }
	FORCEINLINE bool operator!=(const FVector& V) const { return X != V.X || Y != V.Y || Z != V.Z; }

	FORCEINLINE float& operator[](int32 Index)
	{
		return (&X)[Index];
	}

	FORCEINLINE float operator[](int32 Index) const
	{
		return (&X)[Index];
	}

	FORCEINLINE float& Component(int32 Index) { return (&X)[Index]; }
	FORCEINLINE float Component(int32 Index) const { return (&X)[Index]; }

public:
	FORCEINLINE static float DotProduct(const FVector& A, const FVector& B) { return A | B; }
	FORCEINLINE static FVector CrossProduct(const FVector& A, const FVector& B) { return A ^ B; }

	FORCEINLINE static float DistSquared(const FVector& V1, const FVector& V2)
	{
		return FMath::Square(V2.X - V1.X) + FMath::Square(V2.Y - V1.Y) + FMath::Square(V2.Z - V1.Z);
	}

	FORCEINLINE static float DistSquared2D(const FVector& V1, const FVector& V2)
	{
		return FMath::Square(V2.X - V1.X) + FMath::Square(V2.Y - V1.Y);
	}

	FORCEINLINE static float Dist(const FVector& V1, const FVector& V2) { return FMath::Sqrt(DistSquared(V1, V2)); }
	FORCEINLINE static float Distance(const FVector& V1, const FVector& V2) { return Dist(V1, V2); }
	FORCEINLINE static float Dist2D(const FVector& V1, const FVector& V2) { return FMath::Sqrt(DistSquared2D(V1, V2)); }
	FORCEINLINE static float DistXY(const FVector& V1, const FVector& V2) { return Dist2D(V1, V2); }
	FORCEINLINE static float DistSquaredXY(const FVector& V1, const FVector& V2) { return DistSquared2D(V1, V2); }

	FORCEINLINE static float Triple(const FVector& X, const FVector& Y, const FVector& Z)
	{
		return (X.X * (Y.Y * Z.Z - Y.Z * Z.Y))
			+ (X.Y * (Y.Z * Z.X - Y.X * Z.Z))
			+ (X.Z * (Y.X * Z.Y - Y.Y * Z.X));
	}

	FORCEINLINE static bool Parallel(const FVector& Normal1, const FVector& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL)
	{
		const float NormalDot = Normal1 | Normal2;
		return FMath::Abs(NormalDot) >= ParallelCosineThreshold;
	}

	FORCEINLINE static bool Coincident(const FVector& Normal1, const FVector& Normal2, float ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL)
	{
		const float NormalDot = Normal1 | Normal2;
		return NormalDot >= ParallelCosineThreshold;
	}

	FORCEINLINE static bool Orthogonal(const FVector& Normal1, const FVector& Normal2, float OrthogonalCosineThreshold = THRESH_NORMALS_ARE_ORTHOGONAL)
	{
		const float NormalDot = Normal1 | Normal2;
		return FMath::Abs(NormalDot) <= OrthogonalCosineThreshold;
	}

	FORCEINLINE static bool PointsAreSame(const FVector& P, const FVector& Q)
	{
		float Temp;
		Temp = P.X - Q.X;
		if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
		{
			Temp = P.Y - Q.Y;
			if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
			{
				Temp = P.Z - Q.Z;
				if ((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
				{
					return true;
				}
			}
		}
		return false;
	}

	FORCEINLINE static bool PointsAreNear(const FVector& Point1, const FVector& Point2, float Dist)
	{
		float Temp;
		Temp = (Point1.X - Point2.X); if (FMath::Abs(Temp) >= Dist) return false;
		Temp = (Point1.Y - Point2.Y); if (FMath::Abs(Temp) >= Dist) return false;
		Temp = (Point1.Z - Point2.Z); if (FMath::Abs(Temp) >= Dist) return false;
		return true;
	}

public:
	FORCEINLINE void Set(float InX, float InY, float InZ)
	{
		X = InX;
		Y = InY;
		Z = InZ;
	}

	FORCEINLINE bool Equals(const FVector& V, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(X - V.X) <= Tolerance && FMath::Abs(Y - V.Y) <= Tolerance && FMath::Abs(Z - V.Z) <= Tolerance;
	}

	FORCEINLINE bool AllComponentsEqual(float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(X - Y) <= Tolerance && FMath::Abs(X - Z) <= Tolerance && FMath::Abs(Y - Z) <= Tolerance;
	}

	FORCEINLINE float GetMax() const { return FMath::Max(FMath::Max(X, Y), Z); }
	FORCEINLINE float GetAbsMax() const { return FMath::Max(FMath::Max(FMath::Abs(X), FMath::Abs(Y)), FMath::Abs(Z)); }
	FORCEINLINE float GetMin() const { return FMath::Min(FMath::Min(X, Y), Z); }
	FORCEINLINE float GetAbsMin() const { return FMath::Min(FMath::Min(FMath::Abs(X), FMath::Abs(Y)), FMath::Abs(Z)); }

	FORCEINLINE FVector ComponentMin(const FVector& Other) const { return FVector(FMath::Min(X, Other.X), FMath::Min(Y, Other.Y), FMath::Min(Z, Other.Z)); }
	FORCEINLINE FVector ComponentMax(const FVector& Other) const { return FVector(FMath::Max(X, Other.X), FMath::Max(Y, Other.Y), FMath::Max(Z, Other.Z)); }

	FORCEINLINE FVector GetAbs() const { return FVector(FMath::Abs(X), FMath::Abs(Y), FMath::Abs(Z)); }

	FORCEINLINE float Size() const { return FMath::Sqrt(X * X + Y * Y + Z * Z); }
	float SizeSquared() const;
	FORCEINLINE float Size2D() const { return FMath::Sqrt(X * X + Y * Y); }
	FORCEINLINE float SizeSquared2D() const { return X * X + Y * Y; }

	FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(X) <= Tolerance && FMath::Abs(Y) <= Tolerance && FMath::Abs(Z) <= Tolerance;
	}

	bool IsZero() const {
		return X == 0 && Y == 0 && Z == 0;
	}

	FORCEINLINE bool IsUnit(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(1.0f - SizeSquared()) < LengthSquaredTolerance;
	}

	FORCEINLINE bool IsNormalized() const
	{
		return (FMath::Abs(1.f - SizeSquared()) < THRESH_VECTOR_NORMALIZED);
	}

	bool Normalize(float Tolerance = SMALL_NUMBER)
	{
		const float SquareSum = X * X + Y * Y + Z * Z;
		if (SquareSum > Tolerance)
		{
			const float Scale = FMath::InvSqrt(SquareSum);
			X *= Scale; Y *= Scale; Z *= Scale;
			return true;
		}
		return false;
	}

	FVector GetSafeNormal(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y + Z * Z;

		if (SquareSum == 1.f)
		{
			return *this;
		}
		else if (SquareSum < Tolerance)
		{
			return FVector(0.f, 0.f, 0.f);
		}
		const float Scale = FMath::InvSqrt(SquareSum);
		return FVector(X * Scale, Y * Scale, Z * Scale);
	}

	FVector GetSafeNormal2D(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y;

		if (SquareSum == 1.f)
		{
			if (Z == 0.f)
			{
				return *this;
			}
			return FVector(X, Y, 0.f);
		}
		else if (SquareSum < Tolerance)
		{
			return FVector(0.f, 0.f, 0.f);
		}
		const float Scale = FMath::InvSqrt(SquareSum);
		return FVector(X * Scale, Y * Scale, 0.f);
	}

	FORCEINLINE FVector GetUnsafeNormal() const
	{
		const float Scale = FMath::InvSqrt(X * X + Y * Y + Z * Z);
		return FVector(X * Scale, Y * Scale, Z * Scale);
	}

	FORCEINLINE FVector GetSignVector() const
	{
		return FVector(
			FMath::FloatSelect(X, 1.f, -1.f),
			FMath::FloatSelect(Y, 1.f, -1.f),
			FMath::FloatSelect(Z, 1.f, -1.f));
	}

	FORCEINLINE FVector Projection() const
	{
		const float RZ = 1.f / Z;
		return FVector(X * RZ, Y * RZ, 1);
	}

	FORCEINLINE FVector GridSnap(const float& GridSz) const
	{
		return FVector(FMath::GridSnap(X, GridSz), FMath::GridSnap(Y, GridSz), FMath::GridSnap(Z, GridSz));
	}

	FVector GetClampedToSize(float Min, float Max) const
	{
		float VecSize = Size();
		const FVector VecDir = (VecSize > SMALL_NUMBER) ? (*this / VecSize) : FVector(0.f, 0.f, 0.f);

		VecSize = FMath::Clamp(VecSize, Min, Max);

		return VecDir * VecSize;
	}

	FVector GetClampedToSize2D(float Min, float Max) const
	{
		float VecSize2D = Size2D();
		const FVector VecDir = (VecSize2D > SMALL_NUMBER) ? (*this / VecSize2D) : FVector(0.f, 0.f, 0.f);

		VecSize2D = FMath::Clamp(VecSize2D, Min, Max);

		return FVector(VecSize2D * VecDir.X, VecSize2D * VecDir.Y, Z);
	}

	FVector GetClampedToMaxSize(float MaxSize) const
	{
		if (MaxSize < KINDA_SMALL_NUMBER)
		{
			return FVector(0.f, 0.f, 0.f);
		}

		const float VSq = SizeSquared();
		if (VSq > FMath::Square(MaxSize))
		{
			const float Scale = MaxSize * FMath::InvSqrt(VSq);
			return FVector(X * Scale, Y * Scale, Z * Scale);
		}
		return *this;
	}

	FVector GetClampedToMaxSize2D(float MaxSize) const
	{
		if (MaxSize < KINDA_SMALL_NUMBER)
		{
			return FVector(0.f, 0.f, Z);
		}

		const float VSq2D = SizeSquared2D();
		if (VSq2D > FMath::Square(MaxSize))
		{
			const float Scale = MaxSize * FMath::InvSqrt(VSq2D);
			return FVector(X * Scale, Y * Scale, Z);
		}
		return *this;
	}

	FORCEINLINE FVector Reciprocal() const
	{
		FVector RecVector;
		RecVector.X = (X != 0.f) ? 1.f / X : BIG_NUMBER;
		RecVector.Y = (Y != 0.f) ? 1.f / Y : BIG_NUMBER;
		RecVector.Z = (Z != 0.f) ? 1.f / Z : BIG_NUMBER;
		return RecVector;
	}

	FORCEINLINE FVector MirrorByVector(const FVector& MirrorNormal) const
	{
		return *this - MirrorNormal * (2.f * (*this | MirrorNormal));
	}

	FVector RotateAngleAxis(const float AngleDeg, const FVector& Axis) const
	{
		float S, C;
		FMath::SinCos(&S, &C, FMath::DegreesToRadians(AngleDeg));

		const float XX = Axis.X * Axis.X;
		const float YY = Axis.Y * Axis.Y;
		const float ZZ = Axis.Z * Axis.Z;

		const float XY = Axis.X * Axis.Y;
		const float YZ = Axis.Y * Axis.Z;
		const float ZX = Axis.Z * Axis.X;

		const float XS = Axis.X * S;
		const float YS = Axis.Y * S;
		const float ZS = Axis.Z * S;

		const float OMC = 1.f - C;

		return FVector(
			(OMC * XX + C) * X + (OMC * XY - ZS) * Y + (OMC * ZX + YS) * Z,
			(OMC * XY + ZS) * X + (OMC * YY + C) * Y + (OMC * YZ - XS) * Z,
			(OMC * ZX - YS) * X + (OMC * YZ + XS) * Y + (OMC * ZZ + C) * Z);
	}

	FORCEINLINE FVector ProjectOnTo(const FVector& A) const
	{
		return (A * ((*this | A) / (A | A)));
	}

	FORCEINLINE FVector ProjectOnToNormal(const FVector& Normal) const
	{
		return (Normal * (*this | Normal));
	}

	FORCEINLINE float HeadingAngle() const
	{
		FVector PlaneDir = *this;
		PlaneDir.Z = 0.f;
		PlaneDir = PlaneDir.GetSafeNormal();

		float Angle = FMath::Acos(PlaneDir.X);
		if (PlaneDir.Y < 0.0f)
		{
			Angle *= -1.0f;
		}

		return Angle;
	}

	FORCEINLINE float CosineAngle2D(FVector B) const
	{
		FVector A(*this);
		A.Z = 0.0f;
		B.Z = 0.0f;
		A.Normalize();
		B.Normalize();
		return A | B;
	}

	FRotator Rotation() const;
	FRotator ToOrientationRotator() const;
	FQuat ToOrientationQuat() const;

	std::string FormatToString() const {
		return std::format("X={}, Y={}, Z={}", X, Y, Z);
	}
};

inline const FVector FVector::ZeroVector(0.f, 0.f, 0.f);
inline const FVector FVector::OneVector(1.f, 1.f, 1.f);
inline const FVector FVector::UpVector(0.f, 0.f, 1.f);
inline const FVector FVector::DownVector(0.f, 0.f, -1.f);
inline const FVector FVector::ForwardVector(1.f, 0.f, 0.f);
inline const FVector FVector::BackwardVector(-1.f, 0.f, 0.f);
inline const FVector FVector::RightVector(0.f, 1.f, 0.f);
inline const FVector FVector::LeftVector(0.f, -1.f, 0.f);

FORCEINLINE FVector::FVector()
{
	X = 0;
	Y = 0;
	Z = 0;
}

FORCEINLINE FVector::FVector(float InF)
	: X(InF), Y(InF), Z(InF)
{

}

FORCEINLINE FVector::FVector(float InX, float InY, float InZ)
	: X(InX), Y(InY), Z(InZ)
{

}

FORCEINLINE float FVector::operator|(const FVector& V) const
{
	return X * V.X + Y * V.Y + Z * V.Z;
}

FORCEINLINE FVector FVector::operator+(const FVector& V) const
{
	return FVector(X + V.X, Y + V.Y, Z + V.Z);
}

FORCEINLINE FVector FVector::operator-(const FVector& V) const
{
	return FVector(X - V.X, Y - V.Y, Z - V.Z);
}

FORCEINLINE FVector FVector::operator+(float Bias) const
{
	return FVector(X + Bias, Y + Bias, Z + Bias);
}

FORCEINLINE FVector FVector::operator*(float Scale) const
{
	return FVector(X * Scale, Y * Scale, Z * Scale);
}

FORCEINLINE FVector FVector::operator*(const FVector& V) const
{
	return FVector(X * V.X, Y * V.Y, Z * V.Z);
}

FORCEINLINE FVector FVector::operator+=(const FVector& V)
{
	X += V.X; Y += V.Y; Z += V.Z;
	return *this;
}

FORCEINLINE float FVector::SizeSquared() const
{
	return X * X + Y * Y + Z * Z;
}

FORCEINLINE FVector operator*(float Scale, const FVector& V)
{
	return V * Scale;
}

using FVector3f = FVector;

struct FVector3d
{
	double X;
	double Y;
	double Z;

	FVector3d() : X(0), Y(0), Z(0) {}
	FVector3d(double InX, double InY, double InZ) : X(InX), Y(InY), Z(InZ) {}
	explicit FVector3d(const FVector& V) : X(V.X), Y(V.Y), Z(V.Z) {}

	FVector ToFloat() const { return FVector((float)X, (float)Y, (float)Z); }
};

static_assert(sizeof(FVector) == 0xC, "FVector layout broke: UE 4.x expects three floats (0xC)");
static_assert(sizeof(FVector3d) == 0x18, "FVector3d layout broke: UE 5.x expects three doubles (0x18)");
