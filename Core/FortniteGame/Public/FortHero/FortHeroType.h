#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortWorker/FortWorkerType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/AssetPtr.h"

class UFortWorldItemDefinition;
class UFortHeroSpecialization;

struct FHeroItem {
public:
	DefineUnrealStruct(FHeroItem);

	DefineStructProperty(TSoftObjectPtr<UFortWorldItemDefinition>, Item);
};

class UFortHeroType : public UFortWorkerType {
public:
	DefineUnrealClass(UFortHeroType);

	DefineUProperty(TArray<TSoftObjectPtr<UFortHeroSpecialization>>, Specializations);
	DefineUProperty(TArray<FHeroItem>, HeroItems)
public:
	TArray<TAssetPtr<UFortHeroSpecialization>>& GetSpecializationsAssetPtr() {
		TArray<TSoftObjectPtr<UFortHeroSpecialization>>& SpecializationsSoftPtr = Specializations;
		return *reinterpret_cast<TArray<TAssetPtr<UFortHeroSpecialization>>*>(&SpecializationsSoftPtr);
	}
};