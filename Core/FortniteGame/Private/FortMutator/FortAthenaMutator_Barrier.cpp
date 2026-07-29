#include "pch.h"
#include "FortniteGame/Public/FortMutator/FortAthenaMutator_Barrier.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FortniteGame/Public/BuildingActor/AthenaBarrierObjective.h"

void AFortAthenaMutator_Barrier::CheckHealthThreshold(uint8 TeamNum)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("CheckHealthThreshold");

	if (!Func) {
		Log("AFortAthenaMutator_Barrier::CheckHealthThreshold: Failed to find function!");
		return;
	}

	return Call<void>(Func, TeamNum);
}

void AFortAthenaMutator_Barrier::OnGamePhaseStepChanged(uint8 GamePhaseStep)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnGamePhaseStepChanged");

	if (!Func) {
		Log("AFortAthenaMutator_Barrier::OnGamePhaseStepChanged: Failed to find function!");
		return;
	}

	return Call<void>(Func, GamePhaseStep);
}

void AFortAthenaMutator_Barrier::OnMutatorGameplayEvent(int32 EventId, int32 EventParam1, int32 EventParam2, int32 EventParam3)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnMutatorGameplayEvent");

	if (!Func) {
		Log("AFortAthenaMutator_Barrier::OnMutatorGameplayEvent: Failed to find function!");
		return;
	}

	return Call<void>(Func, EventId, EventParam1, EventParam2, EventParam3);
}

void AFortAthenaMutator_Barrier::SpawnModeObjectives()
{
	if (!CachedGameState)
		return;

	AFortGameStateAthena* GameState = (AFortGameStateAthena*)UGameplayStatics::GetGameState(this);
	 
	FAircraftFlightInfo& FlightPath = GameState->FlightPathMidLine;

	FVector WallStart = FlightPath.FlightStartLocation;
	WallStart.Z = 0.0;

	FVector WallEnd = FlightPath.GetFlightEnd();
	WallEnd.Z = 0.0;

	SpawnBarrier(&WallStart, &WallEnd);
}

void AFortAthenaMutator_Barrier::SpawnBarrier(const FVector* WallStart, const FVector* WallEnd)
{
	if (!BigBaseWallClass || !GetWorld())
		return;

	FVector MidPoint = (*WallStart + *WallEnd) * 0.5f;
	FVector GroundLocation = UFortKismetLibrary::FindStaticGroundLocationAt(GetWorld(), MidPoint, nullptr, -9800.0f, 20000.0f);
	float ZLevel = GroundLocation.Z;

	BigBaseWall = AAthenaBigBaseWall::CreateWall(GetWorld(), BigBaseWallClass, *WallStart, *WallEnd, ZLevel);
}

void AFortAthenaMutator_Barrier::BeginPlay(AFortAthenaMutator_Barrier* This)
{
	BeginPlayOG(This);

	if (!This->BigBaseWall) // bSpawnedModeObjects was added later or its just unreflected ig
		This->SpawnModeObjectives();
}

void AFortAthenaMutator_Barrier::SpawnObjectiveActor(TSubclassOf<AAthenaBarrierFlag> InActorClass, FVector InActorLocation, FRotator InActorRotation, FBarrierTeamState* TeamState)
{
	FTransform Transform(InActorRotation, InActorLocation, FVector(1, 1, 1));
	auto Flag = (AAthenaBarrierFlag*)UFortKismetLibrary::SpawnBuildingGameplayActor(InActorClass, Transform, this);

	if (!Flag)
		return;

	Flag->OnRep_FoodTeam();
	Flag->OnRep_CurrentState();

	auto Objective = Flag->GetObjectiveActor();
	if (Objective)
	{
		Objective->OnRep_FoodTeam();
		Objective->OnRep_HeadRotationYaw();
		Objective->OnRep_ObjectiveDamageState();
	}

	TeamState->ObjectiveFlag = Flag;
	TeamState->ObjectiveObject = Objective;
}

void AFortAthenaMutator_Barrier::execOnGamePhaseStepChanged(UObject* Object, FFrame& Stack) {
	execOnGamePhaseStepChangedOG(Object, Stack);
	AFortAthenaMutator_Barrier* Mutator = (AFortAthenaMutator_Barrier*)Object;

	AFortGameStateAthena* GameState = (AFortGameStateAthena*)UGameplayStatics::GetGameState(Object);
	EAthenaGamePhaseStep GamePhaseStep = (EAthenaGamePhaseStep)GameState->GamePhaseStep;

	if (GamePhaseStep == EAthenaGamePhaseStep::BusLocked)
	{
		FVector WallLoc = Mutator->BigBaseWall->K2_GetActorLocation();
		FVector Right = Mutator->BigBaseWall->GetActorRightVector();
		float Dist = Mutator->ObjectiveDistanceFromWall.Evaluate();
		float ZOff = Mutator->ObjectiveZOffset.Evaluate();

		FVector Loc0 = WallLoc + Right * Dist + FVector(0, 0, ZOff);
		FVector Loc1 = WallLoc - Right * Dist + FVector(0, 0, ZOff);

		FVector Ground0 = UFortKismetLibrary::FindStaticGroundLocationAt(UWorld::GetWorld(), Loc0, nullptr, -9800.0f, 20000.0f);
		FVector Ground1 = UFortKismetLibrary::FindStaticGroundLocationAt(UWorld::GetWorld(), Loc1, nullptr, -9800.0f, 20000.0f);

		FVector Dir01 = Ground1 - Ground0;
		FVector Dir10 = Ground0 - Ground1;

		FRotator Rot0(0.f, FMath::RadiansToDegrees(FMath::Atan2(Dir01.Y, Dir01.X)), 0.f);
		FRotator Rot1(0.f, FMath::RadiansToDegrees(FMath::Atan2(Dir10.Y, Dir10.X)), 0.f);

		Mutator->SpawnObjectiveActor(Mutator->ObjectiveFlag, Ground0, Rot0, &Mutator->Team_0_State);
		Mutator->SpawnObjectiveActor(Mutator->ObjectiveFlag, Ground1, Rot1, &Mutator->Team_1_State);
	}
}



void AFortAthenaMutator_Barrier::Hook()
{
	if (Finder::FindAActor_BeginPlayVFT()) {
		HookEveryVTableIdx(AFortAthenaMutator_Barrier::StaticClass(), Finder::FindAActor_BeginPlayVFT(), BeginPlay, (LPVOID*)&BeginPlayOG);
	}
	ExecHook("Function /Script/FortniteGame.FortAthenaMutator_Barrier.OnGamePhaseStepChanged", execOnGamePhaseStepChanged, execOnGamePhaseStepChangedOG);

}