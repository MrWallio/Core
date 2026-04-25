#pragma once
#include "pch.h"

#include "Pawn.h"

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
};