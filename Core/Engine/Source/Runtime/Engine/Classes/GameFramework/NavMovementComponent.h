#pragma once
#include "pch.h"

#include "MovementComponent.h"

class UNavMovementComponent : public UMovementComponent {
public:
	DefineUnrealClass(UNavMovementComponent);

	DefineBitfieldUProperty(bUpdateNavAgentWithOwnersCollision);
};
