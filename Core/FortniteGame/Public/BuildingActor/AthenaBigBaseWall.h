#pragma once
#include "pch.h"

#include "BuildingGameplayActor.h"
#include "FortniteGame/Public/FortMutator/BarrierTypes.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

class AAthenaBigBaseWall : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaBigBaseWall);

	DefineUProperty(float, WallGravity);
	DefineUProperty(float, TimeUntilWallComesDown);
	DefineUProperty(bool, bResetBool);
	DefineUProperty(EBarrierState, BarrierState);
	DefineUProperty(FScalableFloat, ZKillLevel);
public:
	void OnRep_BarrierState();

	void OnRep_ResetBool();

	void OnRep_TimeUntilWallComesDown();

	void OnRep_WallGravity();

	static AAthenaBigBaseWall* CreateWall(UWorld* World, TSubclassOf<AAthenaBigBaseWall> BigBaseWallClass, const FVector& MidlineStart, const FVector& MidlineEnd, float ZLevel);
};
