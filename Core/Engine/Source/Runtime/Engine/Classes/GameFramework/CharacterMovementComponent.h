#pragma once
#include "pch.h"

#include "PawnMovementComponent.h"
#include "Character.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineTypes.h"

class ACharacter;

class UCharacterMovementComponent : public UPawnMovementComponent {
public:
	DefineUnrealClass(UCharacterMovementComponent);

	DefineUProperty(ACharacter*, CharacterOwner);
	DefineUProperty(float, GravityScale);
	DefineUProperty(float, MaxStepHeight);
	DefineUProperty(float, JumpZVelocity);
	DefineUProperty(float, JumpOffJumpZFactor);
	DefineUProperty(float, WalkableFloorAngle);
	DefineUProperty(float, WalkableFloorZ);
	DefineUProperty(uint8, MovementMode);
	DefineUProperty(uint8, CustomMovementMode);
	DefineUProperty(float, GroundFriction);
	DefineUProperty(float, MaxWalkSpeed);
	DefineUProperty(float, MaxWalkSpeedCrouched);
	DefineUProperty(float, MaxSwimSpeed);
	DefineUProperty(float, MaxFlySpeed);
	DefineUProperty(float, MaxCustomMovementSpeed);
	DefineUProperty(float, MaxAcceleration);
	DefineUProperty(float, BrakingFriction);
	DefineUProperty(float, BrakingDecelerationWalking);
	DefineUProperty(float, BrakingDecelerationFalling);
	DefineUProperty(float, BrakingDecelerationSwimming);
	DefineUProperty(float, BrakingDecelerationFlying);
	DefineUProperty(float, AirControl);
	DefineUProperty(float, AirControlBoostMultiplier);
	DefineUProperty(float, AirControlBoostVelocityThreshold);
	DefineUProperty(float, Mass);
	DefineUProperty(float, CrouchedHalfHeight);
	DefineUProperty(FRotator, RotationRate);
	DefineBitfieldUProperty(bOrientRotationToMovement);
	DefineBitfieldUProperty(bUseControllerDesiredRotation);
	DefineBitfieldUProperty(bCheatFlying);
public:
	FORCEINLINE ACharacter* GetCharacterOwner() const { return CharacterOwner; }

	FORCEINLINE bool IsMovingOnGround() const { return (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking) && UpdatedComponent; }
	FORCEINLINE bool IsWalking() const { return IsMovingOnGround(); }
	FORCEINLINE bool IsFalling() const { return MovementMode == MOVE_Falling && UpdatedComponent; }
	FORCEINLINE bool IsSwimming() const { return MovementMode == MOVE_Swimming && UpdatedComponent; }
	FORCEINLINE bool IsFlying() const { return MovementMode == MOVE_Flying && UpdatedComponent; }
	FORCEINLINE bool IsCrouching() const { return CharacterOwner && CharacterOwner->bIsCrouched; }
};
