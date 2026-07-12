#pragma once
#include "pch.h"

#include "Vector.h"
#include "Box.h"
#include "Sphere.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FBoxSphereBounds
{
public:
	DefineUnrealStruct(FBoxSphereBounds);
public:
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
public:
	FORCEINLINE FBoxSphereBounds() : Origin(0, 0, 0), BoxExtent(0, 0, 0), SphereRadius(0) {}

	FORCEINLINE FBoxSphereBounds(const FVector& InOrigin, const FVector& InBoxExtent, float InSphereRadius)
		: Origin(InOrigin), BoxExtent(InBoxExtent), SphereRadius(InSphereRadius)
	{
	}

	FORCEINLINE FBoxSphereBounds(const FBox& Box, const FSphere& Sphere)
	{
		Box.GetCenterAndExtents(Origin, BoxExtent);
		SphereRadius = FMath::Min(BoxExtent.Size(), (Sphere.Center - Origin).Size() + Sphere.W);
	}

	explicit FBoxSphereBounds(const FBox& Box)
	{
		Box.GetCenterAndExtents(Origin, BoxExtent);
		SphereRadius = BoxExtent.Size();
	}

public:
	FORCEINLINE FBox GetBox() const { return FBox(Origin - BoxExtent, Origin + BoxExtent); }
	FORCEINLINE FSphere GetSphere() const { return FSphere(Origin, SphereRadius); }

	FORCEINLINE FBoxSphereBounds operator+(const FBoxSphereBounds& Other) const
	{
		FBox BoundsBox;
		BoundsBox += (this->Origin - this->BoxExtent);
		BoundsBox += (this->Origin + this->BoxExtent);
		BoundsBox += (Other.Origin - Other.BoxExtent);
		BoundsBox += (Other.Origin + Other.BoxExtent);

		FBoxSphereBounds Result(BoundsBox);
		Result.SphereRadius = FMath::Min(
			Result.BoxExtent.Size(),
			FMath::Max(
				(this->Origin - Result.Origin).Size() + this->SphereRadius,
				(Other.Origin - Result.Origin).Size() + Other.SphereRadius));

		return Result;
	}

	FORCEINLINE FBoxSphereBounds ExpandBy(float ExpandAmount) const
	{
		return FBoxSphereBounds(Origin, BoxExtent + FVector(ExpandAmount, ExpandAmount, ExpandAmount), SphereRadius + ExpandAmount);
	}
};

static_assert(sizeof(FBoxSphereBounds) == 0x1C, "FBoxSphereBounds layout broke: Origin + BoxExtent + SphereRadius expected (0x1C)");
