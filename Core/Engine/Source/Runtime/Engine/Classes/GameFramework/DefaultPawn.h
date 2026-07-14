#pragma once
#include "pch.h"

#include "Pawn.h"

class UPawnMovementComponent;
class UStaticMeshComponent;
class USphereComponent;

class ADefaultPawn : public APawn {
public:
	DefineUnrealClass(ADefaultPawn);

	DefineBitfieldUProperty(bAddDefaultMovementBindings);
};
