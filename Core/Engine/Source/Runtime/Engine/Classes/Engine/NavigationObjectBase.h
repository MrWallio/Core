#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"

class ANavigationObjectBase : public AActor {
public:
	DefineUnrealClass(ANavigationObjectBase);

	DefineUProperty(UCapsuleComponent*, CapsuleComponent);
public:
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
};
