#pragma once
#include "pch.h"

#include "AnimationAsset.h"

class UAnimSequenceBase : public UAnimationAsset {
public:
	DefineUnrealClass(UAnimSequenceBase);

	DefineUProperty(float, SequenceLength);
	DefineUProperty(float, RateScale);
public:
	FORCEINLINE float GetPlayLength() const { return SequenceLength; }
};
