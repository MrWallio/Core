#pragma once
#include "pch.h"

#include "ShapeComponent.h"

class UCapsuleComponent : public UShapeComponent {
public:
	DefineUnrealClass(UCapsuleComponent);

	DefineUProperty(float, CapsuleHalfHeight);
	DefineUProperty(float, CapsuleRadius);
public:
	FORCEINLINE float GetScaledCapsuleHalfHeight() const { return CapsuleHalfHeight * GetRelativeScale3D().Z; }
	FORCEINLINE float GetScaledCapsuleRadius() const { return CapsuleRadius * FMath::Max(GetRelativeScale3D().X, GetRelativeScale3D().Y); }
	FORCEINLINE float GetUnscaledCapsuleHalfHeight() const { return CapsuleHalfHeight; }
	FORCEINLINE float GetUnscaledCapsuleRadius() const { return CapsuleRadius; }
};
