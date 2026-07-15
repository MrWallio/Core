#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class USkeletalMeshComponent;

class UAnimInstance : public UObject {
public:
	DefineUnrealClass(UAnimInstance);
public:
	FORCEINLINE USkeletalMeshComponent* GetOwningComponent() const { return (USkeletalMeshComponent*)GetOuter(); }
};
