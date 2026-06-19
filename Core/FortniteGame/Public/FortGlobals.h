#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UFortGlobals : public UObject {
public:
	DefineUnrealClass(UFortGlobals);

	DefineUProperty(bool, bEnableDBNO);
};