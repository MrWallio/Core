#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class USoundBase : public UObject {
public:
	DefineUnrealClass(USoundBase);

	DefineUProperty(float, Duration);
};
