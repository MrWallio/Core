#pragma once
#include "pch.h"

#include "FortAttributeSet.h"

class UFortHealthSet : public UFortAttributeSet {
public:
	DefineUnrealClass(UFortHealthSet);

	DefineUProperty(FFortGameplayAttributeData, Health);
	DefineUProperty(FFortGameplayAttributeData, MaxHealth);
};