#include "pch.h"
#include "FortniteGame/Public/FortWeapon/FortDecoTool.h"

#include "FortniteGame/Public/BuildingActor/BuildingTrap.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

void AFortDecoTool::ServerSpawnDeco(AFortDecoTool* This, FVector& Location, FRotator& Rotation, ABuildingSMActor* AttachedActor, uint8 InBuildingAttachmentType) {
	Log("ServerSpawnDeco Called!");

	AFortPawn* Pawn = This->Owner->Cast<AFortPawn>();
	if (!Pawn) {
        Log("ServerSpawnDeco: Failed to get Pawn from Owner!");
        return;
    }
    
	AFortPlayerController* PlayerController = Pawn->Controller ? Pawn->Controller->Cast<AFortPlayerController>() : nullptr;
    if (!PlayerController) {
        Log("ServerSpawnDeco: Failed to get PlayerController from Pawn!");
        return;
	}

    ServerSpawnDecoOG(This, Location, Rotation, AttachedActor, InBuildingAttachmentType);

	AFortPlayerStateAthena* FortPlayerStateAthena = PlayerController->PlayerState->Cast<AFortPlayerStateAthena>();
    if (FortPlayerStateAthena) {
		ABuildingTrap* TrapActor = nullptr;
        for (ABuildingSMActor* Attached : AttachedActor->AttachedBuildingActors) {
            if (!Attached)
                continue;

            if (Attached->IsA(ABuildingTrap::StaticClass()) && Attached->Team != FortPlayerStateAthena->TeamIndex) {
                TrapActor = Attached->Cast<ABuildingTrap>();
            }
        }

        if (TrapActor) {
            TrapActor->Team = ((AFortPlayerStateAthena*)PlayerController->PlayerState)->TeamIndex;
            TrapActor->TeamIndex = TrapActor->Team;
			Log("ServerSpawnDeco: Updated trap team to " + std::to_string(TrapActor->Team));
        }
    }
}

bool AFortDecoTool::ShouldAllowServerSpawnDeco(FVector& InLocation, FRotator& InRotation, ABuildingSMActor* AttachedActor, uint8 InBuildingAttachmentType) {
	bool (*&ShouldAllowServerSpawnDecoInternal)(AFortDecoTool*, FVector&, FRotator&, ABuildingSMActor*, uint8) = decltype(ShouldAllowServerSpawnDecoInternal)(VTable[Finder::FindAFortDecoTool_ShouldAllowServerSpawnDecoVFT()]);
    return ShouldAllowServerSpawnDecoInternal(this, InLocation, InRotation, AttachedActor, InBuildingAttachmentType);
}

ABuildingActor* AFortDecoTool::SpawnDeco(
    UClass* SpawnClass,
    FVector& Location,
    FRotator& Rotation,
    ABuildingSMActor* AttachedActor,
    uint8 InBuildingAttachmentType,
    int32 PlacementReason
) {
    ABuildingActor* (*&SpawnDecoInternal)(AFortDecoTool*, UClass*, FVector&, FRotator&, ABuildingSMActor*, uint8, int32) = decltype(SpawnDecoInternal)(VTable[Finder::FindAFortDecoTool_SpawnDecoVFT()]);
    return SpawnDecoInternal(this, SpawnClass, Location, Rotation, AttachedActor, InBuildingAttachmentType, PlacementReason);
}