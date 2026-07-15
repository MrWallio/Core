#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UNetDriver;

class UPendingNetGame : public UObject {
public:
	DefineUnrealClass(UPendingNetGame);

	DefineUProperty(UNetDriver*, NetDriver);
};
