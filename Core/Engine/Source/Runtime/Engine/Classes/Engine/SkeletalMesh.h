#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class USkeleton;

class USkeletalMesh : public UObject {
public:
	DefineUnrealClass(USkeletalMesh);

	DefineUProperty(USkeleton*, Skeleton);
public:
	FORCEINLINE USkeleton* GetSkeleton() const { return Skeleton; }
};
