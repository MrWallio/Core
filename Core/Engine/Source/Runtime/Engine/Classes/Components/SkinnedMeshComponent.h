#pragma once
#include "pch.h"

#include "MeshComponent.h"

class USkeletalMesh;

class USkinnedMeshComponent : public UMeshComponent {
public:
	DefineUnrealClass(USkinnedMeshComponent);

	DefineUProperty(USkeletalMesh*, SkeletalMesh);
public:
	FORCEINLINE USkeletalMesh* GetSkeletalMeshAsset() const { return SkeletalMesh; }
};
