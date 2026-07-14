#pragma once
#include "pch.h"

#include "NavMovementComponent.h"

class APawn;

class UPawnMovementComponent : public UNavMovementComponent {
public:
	DefineUnrealClass(UPawnMovementComponent);

	DefineUProperty(APawn*, PawnOwner);
public:
	FORCEINLINE APawn* GetPawnOwner() const { return PawnOwner; }
};
