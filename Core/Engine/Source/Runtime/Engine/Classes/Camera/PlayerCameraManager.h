#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class APlayerController;

class APlayerCameraManager : public AActor {
public:
	DefineUnrealClass(APlayerCameraManager);

	DefineUProperty(APlayerController*, PCOwner);
	DefineUProperty(float, DefaultFOV);
	DefineUProperty(float, ViewPitchMin);
	DefineUProperty(float, ViewPitchMax);
	DefineUProperty(float, ViewYawMin);
	DefineUProperty(float, ViewYawMax);
	DefineUProperty(float, ViewRollMin);
	DefineUProperty(float, ViewRollMax);
public:
	FORCEINLINE APlayerController* GetOwningPlayerController() const { return PCOwner; }
};