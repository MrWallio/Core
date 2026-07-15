#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UDamageType : public UObject {
public:
	DefineUnrealClass(UDamageType);

	DefineBitfieldUProperty(bCausedByWorld);
	DefineBitfieldUProperty(bScaleMomentumByMass);
	DefineUProperty(float, DamageImpulse);
	DefineBitfieldUProperty(bRadialDamageVelChange);
	DefineUProperty(float, DestructibleImpulse);
	DefineUProperty(float, DestructibleDamageSpreadScale);
	DefineUProperty(float, DamageFalloff);
};
