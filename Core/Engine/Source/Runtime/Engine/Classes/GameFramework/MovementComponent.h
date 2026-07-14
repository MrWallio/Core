#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"

class AActor;
class APhysicsVolume;
class UPrimitiveComponent;
class USceneComponent;
struct FCollisionQueryParams;
struct FCollisionResponseParams;
struct FCollisionShape;

class UMovementComponent : public UActorComponent {
public:
	DefineUnrealClass(UMovementComponent);

	DefineUProperty(USceneComponent*, UpdatedComponent);
	DefineUProperty(UPrimitiveComponent*, UpdatedPrimitive);
	DefineUProperty(FVector, Velocity);
public:
	FORCEINLINE USceneComponent* GetUpdatedComponent() const { return UpdatedComponent; }
	FORCEINLINE FVector GetVelocity() const { return Velocity; }
};