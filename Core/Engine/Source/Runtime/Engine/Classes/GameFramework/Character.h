#pragma once
#include "pch.h"

#include "Pawn.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

class AController;
class FDebugDisplayInfo;
class UAnimMontage;
class UArrowComponent;
class UCapsuleComponent;
class UCharacterMovementComponent;
class UPawnMovementComponent;
class UPrimitiveComponent;
class USkeletalMeshComponent;
struct FAnimMontageInstance;

class ACharacter : public APawn {
public:
	DefineUnrealClass(ACharacter);

	DefineUProperty(USkeletalMeshComponent*, Mesh);
	DefineUProperty(UCharacterMovementComponent*, CharacterMovement);
	DefineUProperty(UCapsuleComponent*, CapsuleComponent);
	DefineUProperty(float, CrouchedEyeHeight);
	DefineUProperty(uint8, ReplicatedMovementMode);
	DefineBitfieldUProperty(bIsCrouched);
	DefineBitfieldUProperty(bPressedJump);
	DefineUProperty(float, JumpKeyHoldTime);
	DefineUProperty(float, JumpMaxHoldTime);
	DefineUProperty(int32, JumpMaxCount);
	DefineUProperty(int32, JumpCurrentCount);
public:
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE UCharacterMovementComponent* GetCharacterMovement() const { return CharacterMovement; }
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
};