#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class USceneComponent;

class AFortSafeZoneIndicator : public AActor {
public:
	DefineUnrealClass(AFortSafeZoneIndicator);

	DefineUProperty(float, LastRadius);
	DefineUProperty(float, NextRadius);
	DefineUProperty(float, Radius);
	DefineUProperty(USceneComponent*, SafeZoneMesh);
public:
	FVector GetSafeZoneCenter();

	float GetSafeZoneRadius();

	bool IsInCurrentSafeZone(const FVector& TestLocation);
};
