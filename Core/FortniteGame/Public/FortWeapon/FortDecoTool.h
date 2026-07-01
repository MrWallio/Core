#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "FortWeapon.h"
#include "FortniteGame/Public/FortEnums.h"

class ABuildingActor;
class ABuildingSMActor;
class UFortItemDefinition;
class AFortDecoHelper;

class AFortDecoTool : public AFortWeapon {
public:
	DefineUnrealClass(AFortDecoTool);

	DefineUProperty(bool, bButtonDown);
	DefineUProperty(UFortItemDefinition*, ItemDefinition);
	DefineUProperty(AFortDecoHelper*, DecoHelper);
	DefineUProperty(AActor*, CarriedActor);
	DefineUProperty(bool, bPlaceCarriedActor);
public:
	static inline void (*ServerSpawnDecoOG)(AFortDecoTool* This, FVector& Location, FRotator& Rotation, ABuildingSMActor* AttachedActor, uint8 InBuildingAttachmentType);
	static void ServerSpawnDeco(AFortDecoTool* This, FVector& Location, FRotator& Rotation, ABuildingSMActor* AttachedActor, uint8 InBuildingAttachmentType);

	bool ShouldAllowServerSpawnDeco(FVector& InLocation, FRotator& InRotation, ABuildingSMActor* AttachedActor, uint8 InBuildingAttachmentType);

	ABuildingActor* SpawnDeco(
		UClass* SpawnClass,
		FVector& Location,
		FRotator& Rotation,
		ABuildingSMActor* AttachedActor,
		uint8 InBuildingAttachmentType,
		int32 PlacementReason
	);

	static void Hook() {
		HookEveryVTable(
			AFortDecoTool::StaticClass(),
			AFortDecoTool::StaticClass()->GetFunction("Function /Script/FortniteGame.FortDecoTool.ServerSpawnDeco"),
			ServerSpawnDeco,
			(LPVOID*)&ServerSpawnDecoOG
		);
	}
};