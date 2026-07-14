#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineTypes.h"

class UAudioComponent;
class UBlueprint;
class UDecalComponent;
class UDialogueWave;
class UParticleSystem;
class UParticleSystemComponent;
class USaveGame;
class USceneComponent;
class USoundAttenuation;
class USoundBase;
class USoundConcurrency;
class UStaticMesh;
class FMemoryReader;
class APlayerController;
class AGameModeBase;
class AGameStateBase;
class APawn;
class UWorld;

struct FDialogueContext;

class UGameplayStatics : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UGameplayStatics);

	static AActor* BeginDeferredActorSpawnFromClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined, AActor* Owner = nullptr);

	static AActor* FinishSpawningActor(AActor* Actor, const FTransform& SpawnTransform);

	static UObject* SpawnObject(TSubclassOf<UObject> ObjectClass, UObject* Outer);

	static float GetTimeSeconds(const UObject* WorldContextObject);

	static void GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>* OutActors);

	static void GetAllActorsWithTag(const UObject* WorldContextObject, FName Tag, TArray<AActor*>* OutActors);

	static APlayerController* GetPlayerController(const UObject* WorldContextObject, int32 PlayerIndex);

	static APawn* GetPlayerPawn(const UObject* WorldContextObject, int32 PlayerIndex);

	static AGameModeBase* GetGameMode(const UObject* WorldContextObject);

	static AGameStateBase* GetGameState(const UObject* WorldContextObject);
};
