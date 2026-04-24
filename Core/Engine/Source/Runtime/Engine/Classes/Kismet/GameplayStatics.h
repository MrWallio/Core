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

struct FDialogueContext;

class UGameplayStatics : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UGameplayStatics);

	/** Spawns an instance of an actor class, but does not automatically run its construction script.  */
	static class AActor* BeginDeferredActorSpawnFromClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined, AActor* Owner = nullptr);

	/** 'Finish' spawning an actor.  This will run the construction script. */
	static class AActor* FinishSpawningActor(class AActor* Actor, const FTransform& SpawnTransform);

	static UObject* SpawnObject(TSubclassOf<UObject> ObjectClass, UObject* Outer);

	static float GetTimeSeconds(const UObject* WorldContextObject);
};