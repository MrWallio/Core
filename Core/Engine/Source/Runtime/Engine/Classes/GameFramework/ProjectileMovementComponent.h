#pragma once
#include "pch.h"

#include "MovementComponent.h"

class UProjectileMovementComponent : public UMovementComponent {
public:
	DefineUnrealClass(UProjectileMovementComponent);

	DefineUProperty(float, InitialSpeed);
	DefineUProperty(float, MaxSpeed);
	DefineUProperty(float, ProjectileGravityScale);
	DefineUProperty(float, Bounciness);
	DefineUProperty(float, Friction);
	DefineUProperty(float, HomingAccelerationMagnitude);
	DefineBitfieldUProperty(bRotationFollowsVelocity);
	DefineBitfieldUProperty(bShouldBounce);
	DefineBitfieldUProperty(bIsHomingProjectile);
};