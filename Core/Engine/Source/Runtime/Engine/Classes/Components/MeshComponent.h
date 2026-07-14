#pragma once
#include "pch.h"

#include "PrimitiveComponent.h"

class UMaterialInterface;

class UMeshComponent : public UPrimitiveComponent {
public:
	DefineUnrealClass(UMeshComponent);

	DefineUProperty(TArray<UMaterialInterface*>, OverrideMaterials);
};
