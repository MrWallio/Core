#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingGameplayActor.h"

#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

AFortPickup* AFortAthenaSupplyDrop::SpawnPickup(UFortWorldItemDefinition* ItemDefinition, int32 NumberToSpawn, AFortPawn* TriggeringPawn, FVector& Position, FVector& Direction) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortAthenaSupplyDrop::SpawnPickup: World is null!");
		return nullptr;
	}

	AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
		World,
		ItemDefinition,
		NumberToSpawn,
		Position,
		Direction,
		-1,
		true,
		true,
		false,
		-1,
		EFortPickupSourceTypeFlag::GetOther(),
		EFortPickupSpawnSource::GetSupplyDrop(),
		nullptr,
		false
	);

	return Pickup;
}

void AFortAthenaSupplyDrop::execSpawnPickup(AFortAthenaSupplyDrop* Context, FFrame& Stack, AFortPickup** Result) {
    UFortWorldItemDefinition* ItemDefinition;
    int32 NumberToSpawn;
    AFortPawn* TriggeringPawn;
    FVector Position;
    FVector Direction;
    Stack.StepCompiledIn(&ItemDefinition);
    Stack.StepCompiledIn(&NumberToSpawn);
    Stack.StepCompiledIn(&TriggeringPawn);
    Stack.StepCompiledIn(&Position);
    Stack.StepCompiledIn(&Direction);
    Stack.IncrementCode();

	*Result = Context->SpawnPickup(ItemDefinition, NumberToSpawn, TriggeringPawn, Position, Direction);
}