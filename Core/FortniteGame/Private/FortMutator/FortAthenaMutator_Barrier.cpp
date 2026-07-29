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


void AFortAthenaMutator_Barrier::execOnGamePhaseStepChanged(UObject* Object, FFrame& Stack) {
	execOnGamePhaseStepChangedOG(Object, Stack);
	AFortAthenaMutator_Barrier* Mutator = (AFortAthenaMutator_Barrier*)Object;

	UWorld* World = UWorld::GetWorld();

	EAthenaGamePhaseStep GamePhaseStep = (EAthenaGamePhaseStep)((AFortGameStateAthena*)UGameplayStatics::GetGameState(Object))->GamePhaseStep;

	// objectives are spawned when the bus spawns so thats probably the best way to spawn objectives universally 
	if (GamePhaseStep == EAthenaGamePhaseStep::BusLocked)
	{

	}
	std::cout << (int)GamePhaseStep << std::endl;
}



void AFortAthenaMutator_Barrier::Hook()
{
	if (Finder::FindAActor_BeginPlayVFT()) {
		HookEveryVTableIdx(AFortAthenaMutator_Barrier::StaticClass(), Finder::FindAActor_BeginPlayVFT(), BeginPlay, (LPVOID*)&BeginPlayOG);
	}
	ExecHook("Function /Script/FortniteGame.FortAthenaMutator_Barrier.OnGamePhaseStepChanged", execOnGamePhaseStepChanged, execOnGamePhaseStepChangedOG);

}