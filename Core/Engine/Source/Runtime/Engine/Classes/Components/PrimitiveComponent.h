#pragma once
#include "pch.h"

#include "SceneComponent.h"

class UPrimitiveComponent : public USceneComponent {
public:
	DefineUnrealClass(UPrimitiveComponent);

	DefineBitfieldUProperty(bGenerateOverlapEvents);
	DefineBitfieldUProperty(bMultiBodyOverlap);
	DefineBitfieldUProperty(bReceivesDecals);
	DefineBitfieldUProperty(bOwnerNoSee);
	DefineBitfieldUProperty(bOnlyOwnerSee);
	DefineUProperty(float, MinDrawDistance);
	DefineUProperty(float, LDMaxDrawDistance);
	DefineUProperty(int32, VisibilityId);
public:
	FORCEINLINE bool GetGenerateOverlapEvents() const { return bGenerateOverlapEvents; }

	void SetPhysicsLinearVelocity(const FVector& NewVel, bool bAddToCurrent, FName BoneName);

	void SetPhysicsAngularVelocityInRadians(const FVector& NewAngVel, bool bAddToCurrent, FName BoneName);
};
