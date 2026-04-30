#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class UBuildingEditModeMetadata;
class UFortWeaponItemDefinition;

class AFortWeapon : public AActor {
public:
	DefineUnrealClass(AFortWeapon);

	DefineUProperty(UFortWeaponItemDefinition*, WeaponData);
};

class AFortWeap_BuildingToolBase : public AFortWeapon {
public:
	DefineUnrealClass(AFortWeap_BuildingToolBase);
};

class AFortWeap_BuildingTool : public AFortWeap_BuildingToolBase {
public:
	DefineUnrealClass(AFortWeap_BuildingTool);

	DefineUProperty(UBuildingEditModeMetadata*, DefaultMetadata);
public:
	void OnRep_DefaultMetadata(UBuildingEditModeMetadata* OldMetadata);
};