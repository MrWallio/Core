#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FVector;
struct FQuat;

struct FRotator
{
public:
    DefineUnrealStruct(FRotator);
public:
    float Pitch;
    float Yaw;
    float Roll;
public:
    static const FRotator ZeroRotator;
public:
    FRotator() {
        Pitch = 0.0f;
        Yaw = 0.0f;
        Roll = 0.0f;
    }

    explicit FORCEINLINE FRotator(float InF)
        : Pitch(InF), Yaw(InF), Roll(InF)
    {
    }

    FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll)
        : Pitch(InPitch), Yaw(InYaw), Roll(InRoll)
    {
    }

public:
    FRotator operator+(const FRotator& R) const { return FRotator(Pitch + R.Pitch, Yaw + R.Yaw, Roll + R.Roll); }
    FRotator operator-(const FRotator& R) const { return FRotator(Pitch - R.Pitch, Yaw - R.Yaw, Roll - R.Roll); }

    FRotator operator*(float Scale) const { return FRotator(Pitch * Scale, Yaw * Scale, Roll * Scale); }

    FRotator operator*=(float Scale)
    {
        Pitch = Pitch * Scale; Yaw = Yaw * Scale; Roll = Roll * Scale;
        return *this;
    }

    bool operator==(const FRotator& R) const { return Pitch == R.Pitch && Yaw == R.Yaw && Roll == R.Roll; }
    bool operator!=(const FRotator& V) const { return Pitch != V.Pitch || Yaw != V.Yaw || Roll != V.Roll; }

    FRotator operator+=(const FRotator& R)
    {
        Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
        return *this;
    }

    FRotator operator-=(const FRotator& R)
    {
        Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
        return *this;
    }

public:
    FORCEINLINE bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const
    {
        return FMath::Abs(NormalizeAxis(Pitch)) <= Tolerance
            && FMath::Abs(NormalizeAxis(Yaw)) <= Tolerance
            && FMath::Abs(NormalizeAxis(Roll)) <= Tolerance;
    }

    FORCEINLINE bool IsZero() const
    {
        return (ClampAxis(Pitch) == 0.f) && (ClampAxis(Yaw) == 0.f) && (ClampAxis(Roll) == 0.f);
    }

    FORCEINLINE bool Equals(const FRotator& R, float Tolerance = KINDA_SMALL_NUMBER) const
    {
        return (FMath::Abs(NormalizeAxis(Pitch - R.Pitch)) <= Tolerance)
            && (FMath::Abs(NormalizeAxis(Yaw - R.Yaw)) <= Tolerance)
            && (FMath::Abs(NormalizeAxis(Roll - R.Roll)) <= Tolerance);
    }

    FORCEINLINE FRotator Add(float DeltaPitch, float DeltaYaw, float DeltaRoll)
    {
        Yaw += DeltaYaw;
        Pitch += DeltaPitch;
        Roll += DeltaRoll;
        return *this;
    }

    FORCEINLINE FRotator GridSnap(const FRotator& RotGrid) const
    {
        return FRotator(
            FMath::GridSnap(Pitch, RotGrid.Pitch),
            FMath::GridSnap(Yaw, RotGrid.Yaw),
            FMath::GridSnap(Roll, RotGrid.Roll));
    }

    FORCEINLINE FRotator Clamp() const
    {
        return FRotator(ClampAxis(Pitch), ClampAxis(Yaw), ClampAxis(Roll));
    }

    FRotator GetNormalized() const
    {
        FRotator Rot = *this;
        Rot.Normalize();
        return Rot;
    }

    FRotator GetDenormalized() const
    {
        FRotator Rot = *this;
        Rot.Pitch = ClampAxis(Rot.Pitch);
        Rot.Yaw = ClampAxis(Rot.Yaw);
        Rot.Roll = ClampAxis(Rot.Roll);
        return Rot;
    }

    void Normalize()
    {
        Pitch = NormalizeAxis(Pitch);
        Yaw = NormalizeAxis(Yaw);
        Roll = NormalizeAxis(Roll);
    }

public:
    static FORCEINLINE float ClampAxis(float Angle)
    {
        Angle = FMath::Fmod(Angle, 360.f);

        if (Angle < 0.f)
        {
            Angle += 360.f;
        }

        return Angle;
    }

    static FORCEINLINE float NormalizeAxis(float Angle)
    {
        Angle = ClampAxis(Angle);

        if (Angle > 180.f)
        {
            Angle -= 360.f;
        }

        return Angle;
    }

public:
    FQuat Quaternion() const;

    FVector Vector() const;
    FVector Euler() const;
    FVector RotateVector(const FVector& V) const;
    FVector UnrotateVector(const FVector& V) const;
    FRotator GetInverse() const;

    std::string FormatToString() const {
        return std::format("P={}, Y={}, R={}", Pitch, Yaw, Roll);
    }
};

inline const FRotator FRotator::ZeroRotator(0.f, 0.f, 0.f);

using FRotator3f = FRotator;

struct FRotator3d
{
    double Pitch;
    double Yaw;
    double Roll;

    FRotator3d() : Pitch(0), Yaw(0), Roll(0) {}
    FRotator3d(double InPitch, double InYaw, double InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}
    explicit FRotator3d(const FRotator& R) : Pitch(R.Pitch), Yaw(R.Yaw), Roll(R.Roll) {}

    FRotator ToFloat() const { return FRotator((float)Pitch, (float)Yaw, (float)Roll); }
};

static_assert(sizeof(FRotator) == 0xC, "FRotator layout broke: UE 4.x expects three floats (0xC)");
static_assert(sizeof(FRotator3d) == 0x18, "FRotator3d layout broke: UE 5.x expects three doubles (0x18)");
