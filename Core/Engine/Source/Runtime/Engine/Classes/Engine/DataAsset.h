#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UDataAsset : public UObject {
public:
	DefineUnrealClass(UDataAsset);
};

class UPrimaryDataAsset : public UDataAsset {
public:
	DefineUnrealClass(UPrimaryDataAsset);
};