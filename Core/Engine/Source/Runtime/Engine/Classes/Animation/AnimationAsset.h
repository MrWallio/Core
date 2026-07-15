#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class USkeleton;

class UAnimationAsset : public UObject {
public:
	DefineUnrealClass(UAnimationAsset);

	DefineUProperty(USkeleton*, Skeleton);
public:
	FORCEINLINE USkeleton* GetSkeleton() const { return Skeleton; }
};
