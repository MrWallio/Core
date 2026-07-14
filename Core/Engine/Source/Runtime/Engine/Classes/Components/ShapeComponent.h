#pragma once
#include "pch.h"

#include "PrimitiveComponent.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"

class UShapeComponent : public UPrimitiveComponent {
public:
	DefineUnrealClass(UShapeComponent);

	DefineUProperty(FColor, ShapeColor);
	DefineBitfieldUProperty(bDrawOnlyIfSelected);
};
