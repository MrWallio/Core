#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Quat.h"
#include "Vector.h"

struct FTransform
{
public:
	DefineUnrealStruct(FTransform);
public:
	FQuat Rotation;
	FVector Translation;
	uint8 Pad_1C[0x4];
	FVector Scale3D;
	uint8 Pad_2C[0x4];
public:
	FORCEINLINE FTransform()
		: Rotation(0.f, 0.f, 0.f, 1.f)
		, Translation(0.f, 0.f, 0.f)
		, Scale3D(1.f, 1.f, 1.f)
	{
	}

	FORCEINLINE explicit FTransform(const FVector& InTranslation)
		: Rotation(0.f, 0.f, 0.f, 1.f)
		, Translation(InTranslation)
		, Scale3D(1.f, 1.f, 1.f)
	{
	}

	FORCEINLINE explicit FTransform(const FQuat& InRotation)
		: Rotation(InRotation)
		, Translation(0.f, 0.f, 0.f)
		, Scale3D(1.f, 1.f, 1.f)
	{
	}

	FORCEINLINE FTransform(const FQuat& InRotation, const FVector& InTranslation, const FVector& InScale3D = FVector(1.f, 1.f, 1.f))
		: Rotation(InRotation)
		, Translation(InTranslation)
		, Scale3D(InScale3D)
	{
	}

	FTransform(const FRotator& InRotation, const FVector& InTranslation, const FVector& InScale3D = FVector(1.f, 1.f, 1.f));

public:
	FORCEINLINE const FQuat& GetRotation() const { return Rotation; }
	FORCEINLINE const FVector& GetTranslation() const { return Translation; }
	FORCEINLINE const FVector& GetLocation() const { return Translation; }
	FORCEINLINE const FVector& GetScale3D() const { return Scale3D; }

	FORCEINLINE void SetRotation(const FQuat& NewRotation) { Rotation = NewRotation; }
	FORCEINLINE void SetTranslation(const FVector& NewTranslation) { Translation = NewTranslation; }
	FORCEINLINE void SetLocation(const FVector& NewLocation) { Translation = NewLocation; }
	FORCEINLINE void SetScale3D(const FVector& NewScale3D) { Scale3D = NewScale3D; }

	FORCEINLINE void SetComponents(const FQuat& InRotation, const FVector& InTranslation, const FVector& InScale3D)
	{
		Rotation = InRotation;
		Translation = InTranslation;
		Scale3D = InScale3D;
	}

	FORCEINLINE void SetIdentity()
	{
		Rotation = FQuat(0.f, 0.f, 0.f, 1.f);
		Translation = FVector(0.f, 0.f, 0.f);
		Scale3D = FVector(1.f, 1.f, 1.f);
	}

public:
	FORCEINLINE bool Equals(const FTransform& Other, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return Rotation.Equals(Other.Rotation, Tolerance)
			&& Translation.Equals(Other.Translation, Tolerance)
			&& Scale3D.Equals(Other.Scale3D, Tolerance);
	}

	FORCEINLINE bool operator==(const FTransform& Other) const
	{
		return Rotation == Other.Rotation && Translation == Other.Translation && Scale3D == Other.Scale3D;
	}

	FORCEINLINE FVector TransformPosition(const FVector& V) const
	{
		return Rotation.RotateVector(Scale3D * V) + Translation;
	}

	FORCEINLINE FVector TransformPositionNoScale(const FVector& V) const
	{
		return Rotation.RotateVector(V) + Translation;
	}

	FORCEINLINE FVector TransformVector(const FVector& V) const
	{
		return Rotation.RotateVector(Scale3D * V);
	}

	FORCEINLINE FVector TransformVectorNoScale(const FVector& V) const
	{
		return Rotation.RotateVector(V);
	}

	FORCEINLINE FVector InverseTransformPosition(const FVector& V) const
	{
		return (Rotation.UnrotateVector(V - Translation)) * GetSafeScaleReciprocal(Scale3D);
	}

	FORCEINLINE FVector InverseTransformVector(const FVector& V) const
	{
		return Rotation.UnrotateVector(V) * GetSafeScaleReciprocal(Scale3D);
	}

	FORCEINLINE FQuat TransformRotation(const FQuat& Q) const { return Rotation * Q; }
	FORCEINLINE FQuat InverseTransformRotation(const FQuat& Q) const { return Rotation.Inverse() * Q; }

	FTransform operator*(const FTransform& Other) const
	{
		FTransform Result;
		Result.Rotation = Other.Rotation * Rotation;
		Result.Scale3D = Scale3D * Other.Scale3D;
		Result.Translation = Other.Rotation.RotateVector(Other.Scale3D * Translation) + Other.Translation;
		return Result;
	}

	FTransform Inverse() const
	{
		const FQuat InvRotation = Rotation.Inverse();
		const FVector InvScale = GetSafeScaleReciprocal(Scale3D);
		const FVector InvTranslation = InvRotation.RotateVector(InvScale * -Translation);
		return FTransform(InvRotation, InvTranslation, InvScale);
	}

	FORCEINLINE FVector GetScaledAxis(int32 InAxis) const
	{
		if (InAxis == 0) return Rotation.GetAxisX() * Scale3D.X;
		if (InAxis == 1) return Rotation.GetAxisY() * Scale3D.Y;
		return Rotation.GetAxisZ() * Scale3D.Z;
	}

	FORCEINLINE bool IsRotationNormalized() const { return Rotation.IsNormalized(); }

	FRotator Rotator() const;

	static FORCEINLINE FVector GetSafeScaleReciprocal(const FVector& InScale, float Tolerance = SMALL_NUMBER)
	{
		FVector SafeReciprocalScale;
		SafeReciprocalScale.X = (FMath::Abs(InScale.X) <= Tolerance) ? 0.f : 1.f / InScale.X;
		SafeReciprocalScale.Y = (FMath::Abs(InScale.Y) <= Tolerance) ? 0.f : 1.f / InScale.Y;
		SafeReciprocalScale.Z = (FMath::Abs(InScale.Z) <= Tolerance) ? 0.f : 1.f / InScale.Z;
		return SafeReciprocalScale;
	}

	static const FTransform Identity;
};

inline const FTransform FTransform::Identity;

using FTransform3f = FTransform;

static_assert(sizeof(FTransform) == 0x30, "FTransform layout broke: aligned Rotation/Translation/Scale3D with pads = 0x30 on x64");
static_assert(offsetof(FTransform, Translation) == 0x10, "FTransform::Translation must sit at 0x10 to match the engine");
static_assert(offsetof(FTransform, Scale3D) == 0x20, "FTransform::Scale3D must sit at 0x20 to match the engine");
