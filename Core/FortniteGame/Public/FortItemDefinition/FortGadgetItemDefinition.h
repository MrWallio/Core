#pragma once
#include "pch.h"

#include "FortWorldItemDefinition.h"

class UFortGadgetItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortGadgetItemDefinition);

	DefineBitfieldUProperty(bDropAllOnEquip);
};

class UAthenaGadgetItemDefinition : public UFortGadgetItemDefinition {
public:
	DefineUnrealClass(UAthenaGadgetItemDefinition);
};