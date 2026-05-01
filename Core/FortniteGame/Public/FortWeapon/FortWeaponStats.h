#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"

class ABuildingSMActor;

struct FFortBaseWeaponStats : public FTableRowBase {
public:
	DefineUnrealStruct(FFortBaseWeaponStats);

	DefineStructProperty(int32, ClipSize);
};

struct FFortRangedWeaponStats : public FFortBaseWeaponStats {
public:
	DefineUnrealStruct(FFortRangedWeaponStats);
};