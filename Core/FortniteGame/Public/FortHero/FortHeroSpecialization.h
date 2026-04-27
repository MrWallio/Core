#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

class UCustomCharacterPart;

class UFortHeroSpecialization : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortHeroSpecialization);

	DefineUProperty(TArray<TSoftObjectPtr<UCustomCharacterPart>>, CharacterParts);
};