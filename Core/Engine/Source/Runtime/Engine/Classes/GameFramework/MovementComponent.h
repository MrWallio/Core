#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"

class AActor;
class APhysicsVolume;
class UPrimitiveComponent;
struct FCollisionQueryParams;
struct FCollisionResponseParams;
struct FCollisionShape;

class UMovementComponent : public UActorComponent {
public:
	DefineUnrealClass(UMovementComponent);
};