#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class AActor;
class UPrimitiveComponent;

struct FHitResult {
public:
	DefineUnrealStruct(FHitResult);
public:
	uint8 OpaqueStorage[0x120]{};
public:
	DefineStructProperty(float, Time);
	DefineStructProperty(float, Distance);
	DefineStructProperty(FVector, Location);
	DefineStructProperty(FVector, ImpactPoint);
	DefineStructProperty(FVector, Normal);
	DefineStructProperty(FVector, ImpactNormal);
	DefineStructProperty(FVector, TraceStart);
	DefineStructProperty(FVector, TraceEnd);
	DefineStructProperty(float, PenetrationDepth);
	DefineStructProperty(int32, Item);
	DefineStructProperty(TWeakObjectPtr<AActor>, Actor);
	DefineStructProperty(TWeakObjectPtr<UPrimitiveComponent>, Component);
	DefineStructProperty(FName, BoneName);
	DefineStructProperty(int32, FaceIndex);
public:
	bool IsValidBlockingHit() const {
		static int32 Offset = -2;
		static uint8 FieldMask = 0;
		if (Offset == -2) {
			UBoolProperty* Prop = (UBoolProperty*)StaticStruct()->FindPropertyByName("bBlockingHit");
			if (Prop) {
				Offset = Prop->Offset_Internal;
				FieldMask = Prop->FieldMask;
			}
			else {
				Offset = -1;
			}
		}
		if (Offset < 0)
			return false;
		return (*((uint8*)this + Offset) & FieldMask) != 0;
	}

	bool bStartPenetrating() const {
		static int32 Offset = -2;
		static uint8 FieldMask = 0;
		if (Offset == -2) {
			UBoolProperty* Prop = (UBoolProperty*)StaticStruct()->FindPropertyByName("bStartPenetrating");
			if (Prop) {
				Offset = Prop->Offset_Internal;
				FieldMask = Prop->FieldMask;
			}
			else {
				Offset = -1;
			}
		}
		if (Offset < 0)
			return false;
		return (*((uint8*)this + Offset) & FieldMask) != 0;
	}

	AActor* GetActor() const {
		return Actor.Get();
	}

	UPrimitiveComponent* GetComponent() const {
		return Component.Get();
	}
};
