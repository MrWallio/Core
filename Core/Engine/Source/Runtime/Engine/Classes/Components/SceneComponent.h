#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"

class APhysicsVolume;
class USceneComponent;
struct FTransform;
struct FHitResult;

class USceneComponent : public UActorComponent {
public:
	DefineUnrealClass(USceneComponent);

	DefineUProperty(USceneComponent*, AttachParent);
	DefineUProperty(FName, AttachSocketName);
	DefineUProperty(TArray<USceneComponent*>, AttachChildren);
	DefineUProperty(FVector, RelativeLocation);
	DefineUProperty(FRotator, RelativeRotation);
	DefineUProperty(FVector, RelativeScale3D);
	DefineUProperty(FVector, ComponentVelocity);
	DefineUProperty(uint8, Mobility);
	DefineBitfieldUProperty(bAbsoluteLocation);
	DefineBitfieldUProperty(bAbsoluteRotation);
	DefineBitfieldUProperty(bAbsoluteScale);
	DefineBitfieldUProperty(bVisible);
	DefineBitfieldUProperty(bHiddenInGame);
public:
	FORCEINLINE USceneComponent* GetAttachParent() const { return AttachParent; }
	FORCEINLINE FName GetAttachSocketName() const { return AttachSocketName; }

	FORCEINLINE FVector GetRelativeLocation() const { return RelativeLocation; }
	FORCEINLINE FRotator GetRelativeRotation() const { return RelativeRotation; }
	FORCEINLINE FVector GetRelativeScale3D() const { return RelativeScale3D; }

	void SetRelativeScale3D(FVector NewScale3D);

	void K2_SetWorldTransform(const FTransform& NewTransform, bool bSweep, FHitResult* SweepHitResult, bool bTeleport);

	FORCEINLINE bool IsVisible() const { return bVisible; }

	USceneComponent* GetAttachmentRoot() const
	{
		const USceneComponent* Top;
		for (Top = this; Top && Top->GetAttachParent(); Top = Top->GetAttachParent());
		return const_cast<USceneComponent*>(Top);
	}

	AActor* GetAttachmentRootActor() const
	{
		const USceneComponent* const AttachmentRootComponent = GetAttachmentRoot();
		return AttachmentRootComponent ? AttachmentRootComponent->GetOwner() : nullptr;
	}

	bool IsAttachedTo(const USceneComponent* TestComp) const
	{
		if (TestComp != nullptr)
		{
			for (const USceneComponent* Comp = this->GetAttachParent(); Comp != nullptr; Comp = Comp->GetAttachParent())
			{
				if (TestComp == Comp)
				{
					return true;
				}
			}
		}
		return false;
	}
};
