#pragma once
#include "pch.h"

#include "PrimitiveComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Materials/MaterialInterface.h"

class UMaterialInterface;

class UMeshComponent : public UPrimitiveComponent {
public:
	DefineUnrealClass(UMeshComponent);

	DefineUProperty(TArray<UMaterialInterface*>, OverrideMaterials);
};
