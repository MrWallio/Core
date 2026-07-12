#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Rotator.h"
#include "Vector.h"

class Error;

struct alignas(0x10) FQuat
{
public:
    DefineUnrealStruct(FQuat);
public:
    float X;
    float Y;
    float Z;
    float W;
public:
    static const FQuat Identity;
public:
    /** Default constructor (no initialization). */
    FORCEINLINE FQuat() {}

    FORCEINLINE FQuat(float InX, float InY, float InZ, float InW)
        : X(InX), Y(InY), Z(InZ), W(InW)
    {
    }

    FQuat(const FRotator& R);

    FQuat(const FVector& Axis, float AngleRad)
    {
        const float half_a = 0.5f * AngleRad;
        float s, c;
        FMath::SinCos(&s, &c, half_a);

        X = s * Axis.X;
        Y = s * Axis.Y;
        Z = s * Axis.Z;
        W = c;
    }

public:
    FORCEINLINE FQuat operator+(const FQuat& Q) const { return FQuat(X + Q.X, Y + Q.Y, Z + Q.Z, W + Q.W); }
    FORCEINLINE FQuat operator-(const FQuat& Q) const { return FQuat(X - Q.X, Y - Q.Y, Z - Q.Z, W - Q.W); }

    FORCEINLINE FQuat operator*(const FQuat& Q) const
    {
        FQuat Result;
        Result.X = (W * Q.X) + (X * Q.W) + (Y * Q.Z) - (Z * Q.Y);
        Result.Y = (W * Q.Y) - (X * Q.Z) + (Y * Q.W) + (Z * Q.X);
        Result.Z = (W * Q.Z) + (X * Q.Y) - (Y * Q.X) + (Z * Q.W);
        Result.W = (W * Q.W) - (X * Q.X) - (Y * Q.Y) - (Z * Q.Z);
        return Result;
    }

    FORCEINLINE FQuat operator*=(const FQuat& Q)
    {
        *this = *this * Q;
        return *this;
    }

    FORCEINLINE FQuat operator*(const float Scale) const { return FQuat(Scale * X, Scale * Y, Scale * Z, Scale * W); }
    FORCEINLINE FQuat operator/(const float Scale) const { const float Recip = 1.0f / Scale; return FQuat(X * Recip, Y * Recip, Z * Recip, W * Recip); }

    FORCEINLINE bool operator==(const FQuat& Q) const { return X == Q.X && Y == Q.Y && Z == Q.Z && W == Q.W; }
    FORCEINLINE bool operator!=(const FQuat& Q) const { return X != Q.X || Y != Q.Y || Z != Q.Z || W != Q.W; }

    FORCEINLINE float operator|(const FQuat& Q) const { return X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W; }

public:
    FORCEINLINE bool Equals(const FQuat& Q, float Tolerance = KINDA_SMALL_NUMBER) const
    {
        return (FMath::Abs(X - Q.X) <= Tolerance && FMath::Abs(Y - Q.Y) <= Tolerance && FMath::Abs(Z - Q.Z) <= Tolerance && FMath::Abs(W - Q.W) <= Tolerance)
            || (FMath::Abs(X + Q.X) <= Tolerance && FMath::Abs(Y + Q.Y) <= Tolerance && FMath::Abs(Z + Q.Z) <= Tolerance && FMath::Abs(W + Q.W) <= Tolerance);
    }

    FORCEINLINE bool IsIdentity(float Tolerance = SMALL_NUMBER) const
    {
        return Equals(FQuat(0.f, 0.f, 0.f, 1.f), Tolerance);
    }

    FORCEINLINE float Size() const { return FMath::Sqrt(X * X + Y * Y + Z * Z + W * W); }
    FORCEINLINE float SizeSquared() const { return X * X + Y * Y + Z * Z + W * W; }

    FORCEINLINE void Normalize(float Tolerance = SMALL_NUMBER)
    {
        const float SquareSum = X * X + Y * Y + Z * Z + W * W;

        if (SquareSum >= Tolerance)
        {
            const float Scale = FMath::InvSqrt(SquareSum);
            X *= Scale; Y *= Scale; Z *= Scale; W *= Scale;
        }
        else
        {
            *this = FQuat(0.f, 0.f, 0.f, 1.f);
        }
    }

    FORCEINLINE FQuat GetNormalized(float Tolerance = SMALL_NUMBER) const
    {
        FQuat Result(*this);
        Result.Normalize(Tolerance);
        return Result;
    }

    FORCEINLINE bool IsNormalized() const
    {
        return (FMath::Abs(1.f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
    }

    FORCEINLINE FQuat Inverse() const
    {
        return FQuat(-X, -Y, -Z, W);
    }

    FORCEINLINE void EnforceShortestArcWith(const FQuat& OtherQuat)
    {
        const float DotResult = (OtherQuat | *this);
        const float Bias = FMath::FloatSelect(DotResult, 1.0f, -1.0f);

        X *= Bias; Y *= Bias; Z *= Bias; W *= Bias;
    }

public:
    FVector RotateVector(const FVector& V) const
    {
        const FVector Q(X, Y, Z);
        const FVector T = 2.f * FVector::CrossProduct(Q, V);
        const FVector Result = V + (W * T) + FVector::CrossProduct(Q, T);
        return Result;
    }

    FVector UnrotateVector(const FVector& V) const
    {
        const FVector Q(-X, -Y, -Z);
        const FVector T = 2.f * FVector::CrossProduct(Q, V);
        const FVector Result = V + (W * T) + FVector::CrossProduct(Q, T);
        return Result;
    }

    FORCEINLINE FVector GetAxisX() const { return RotateVector(FVector(1.f, 0.f, 0.f)); }
    FORCEINLINE FVector GetAxisY() const { return RotateVector(FVector(0.f, 1.f, 0.f)); }
    FORCEINLINE FVector GetAxisZ() const { return RotateVector(FVector(0.f, 0.f, 1.f)); }

    FORCEINLINE FVector GetForwardVector() const { return GetAxisX(); }
    FORCEINLINE FVector GetRightVector() const { return GetAxisY(); }
    FORCEINLINE FVector GetUpVector() const { return GetAxisZ(); }

    FORCEINLINE FVector Vector() const { return GetAxisX(); }

    FVector GetRotationAxis() const
    {
        const float S = FMath::Sqrt(FMath::Max(1.f - (W * W), 0.f));

        if (S >= 0.0001f)
        {
            return FVector(X / S, Y / S, Z / S);
        }

        return FVector(1.f, 0.f, 0.f);
    }

    FORCEINLINE float GetAngle() const { return 2.f * FMath::Acos(W); }

    float AngularDistance(const FQuat& Q) const
    {
        float InnerProd = X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
        return FMath::Acos((2 * InnerProd * InnerProd) - 1.f);
    }

public:
    static FQuat Slerp_NotNormalized(const FQuat& Quat1, const FQuat& Quat2, float Slerp)
    {
        const float RawCosom = Quat1 | Quat2;
        const float Cosom = FMath::FloatSelect(RawCosom, RawCosom, -RawCosom);

        float Scale0, Scale1;

        if (Cosom < 0.9999f)
        {
            const float Omega = FMath::Acos(Cosom);
            const float InvSin = 1.f / FMath::Sin(Omega);
            Scale0 = FMath::Sin((1.f - Slerp) * Omega) * InvSin;
            Scale1 = FMath::Sin(Slerp * Omega) * InvSin;
        }
        else
        {
            Scale0 = 1.0f - Slerp;
            Scale1 = Slerp;
        }

        Scale1 = FMath::FloatSelect(RawCosom, Scale1, -Scale1);

        FQuat Result;
        Result.X = Scale0 * Quat1.X + Scale1 * Quat2.X;
        Result.Y = Scale0 * Quat1.Y + Scale1 * Quat2.Y;
        Result.Z = Scale0 * Quat1.Z + Scale1 * Quat2.Z;
        Result.W = Scale0 * Quat1.W + Scale1 * Quat2.W;

        return Result;
    }

    static FQuat Slerp(const FQuat& Quat1, const FQuat& Quat2, float Slerp)
    {
        return Slerp_NotNormalized(Quat1, Quat2, Slerp).GetNormalized();
    }

    static FQuat FindBetweenNormals(const FVector& Normal1, const FVector& Normal2)
    {
        const float NormalAB = 1.f;
        return FindBetween_Helper(Normal1, Normal2, NormalAB);
    }

    static FQuat FindBetweenVectors(const FVector& Vector1, const FVector& Vector2)
    {
        const float NormalAB = FMath::Sqrt(Vector1.SizeSquared() * Vector2.SizeSquared());
        return FindBetween_Helper(Vector1, Vector2, NormalAB);
    }

    static FQuat FindBetween(const FVector& Vector1, const FVector& Vector2)
    {
        return FindBetweenVectors(Vector1, Vector2);
    }

private:
    static FQuat FindBetween_Helper(const FVector& A, const FVector& B, float NormAB)
    {
        float W = NormAB + FVector::DotProduct(A, B);
        FQuat Result;

        if (W >= 1e-6f * NormAB)
        {
            Result = FQuat(
                A.Y * B.Z - A.Z * B.Y,
                A.Z * B.X - A.X * B.Z,
                A.X * B.Y - A.Y * B.X,
                W);
        }
        else
        {
            W = 0.f;
            Result = FMath::Abs(A.X) > FMath::Abs(A.Y)
                ? FQuat(-A.Z, 0.f, A.X, W)
                : FQuat(0.f, -A.Z, A.Y, W);
        }

        Result.Normalize();
        return Result;
    }

public:
    FRotator Rotator() const;
    FVector Euler() const;

    std::string FormatToString() const {
        return std::format("X={}, Y={}, Z={}, W={}", X, Y, Z, W);
    }
};

inline const FQuat FQuat::Identity(0.f, 0.f, 0.f, 1.f);

FORCEINLINE FQuat::FQuat(const FRotator& R)
{
    *this = R.Quaternion();
}

using FQuat4f = FQuat;

static_assert(sizeof(FQuat) == 0x10, "FQuat layout broke: UE 4.x expects four floats (0x10)");
static_assert(alignof(FQuat) == 0x10, "FQuat alignment broke: UE aligns quaternions to 16");
