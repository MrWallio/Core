#pragma once
#include "pch.h"

#include "FortWorldItemDefinition.h"

class UFortWeaponItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortWeaponItemDefinition);

	DefineUProperty(FDataTableRowHandle, WeaponStatHandle);
};

class UFortEditToolItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortEditToolItemDefinition);
};

class UFortBuildingItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortBuildingItemDefinition);
};

class UFortDecoItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortDecoItemDefinition);
};

class UFortTrapItemDefinition : public UFortDecoItemDefinition {
public:
	DefineUnrealClass(UFortTrapItemDefinition);
};