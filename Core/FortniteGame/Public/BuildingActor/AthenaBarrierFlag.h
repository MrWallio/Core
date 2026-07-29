#pragma once
#include "pch.h"

#include "BuildingGameplayActor.h"
#include "FortniteGame/Public/FortMutator/BarrierTypes.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class AAthenaBarrierObjective;
class UStaticMesh;

class AAthenaBarrierFlag : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaBarrierFlag);

	DefineUProperty(EBarrierFlagState, CurrentState);
	DefineUProperty(EBarrierFoodTeam, FoodTeam);
	DefineUProperty(FBarrierFlagDisplayData, FoodDisplayData);
public:
	void OnRep_CurrentState();

	void OnRep_FoodTeam();

	UStaticMesh* GetHeadMesh();

	FVector GetMeshScale();

	AAthenaBarrierObjective* GetObjectiveActor();

	void SetFoodTeam(EBarrierFoodTeam NewFoodTeam);
};
