#pragma once
#include "pch.h"

#include "MaterialInterface.h"

class UMaterialInstance : public UMaterialInterface {
public:
	DefineUnrealClass(UMaterialInstance);

	DefineUProperty(UMaterialInterface*, Parent);
public:
	FORCEINLINE UMaterialInterface* GetParent() const { return Parent; }
};
