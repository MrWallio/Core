#pragma once
#include "pch.h"

#include "AnimCompositeBase.h"

class UAnimMontage : public UAnimCompositeBase {
public:
	DefineUnrealClass(UAnimMontage);

	DefineUProperty(float, BlendInTime);
	DefineUProperty(float, BlendOutTime);
	DefineUProperty(float, BlendOutTriggerTime);
};
