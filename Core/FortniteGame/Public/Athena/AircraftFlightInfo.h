#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetSerialization.h"

struct FAircraftFlightInfo {
public:
	DefineUnrealStruct(FAircraftFlightInfo);

	DefineStructProperty(FVector_NetQuantize100, FlightStartLocation);
	DefineStructProperty(FRotator, FlightStartRotation);
	DefineStructProperty(float, FlightSpeed);
	DefineStructProperty(float, TimeTillFlightEnd);
	DefineStructProperty(float, TimeTillDropStart);
	DefineStructProperty(float, TimeTillDropEnd);
public:
	FVector GetFlightEnd();
};
