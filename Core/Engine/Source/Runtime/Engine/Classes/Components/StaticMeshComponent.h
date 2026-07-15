#pragma once
#include "pch.h"

#include "MeshComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/StaticMesh.h"

class UStaticMesh;

class UStaticMeshComponent : public UMeshComponent {
public:
	DefineUnrealClass(UStaticMeshComponent);

	DefineUProperty(UStaticMesh*, StaticMesh);
public:
	FORCEINLINE UStaticMesh* GetStaticMesh() const { return StaticMesh; }
};
