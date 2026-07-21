#pragma once
#include "pch.h"

#include "BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/TimerHandle.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"

class AActor;
class ACameraActor;
class APlayerController;
class UPrimitiveComponent;
class USceneComponent;
struct FTimerHandle;

class UKismetSystemLibrary : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UKismetSystemLibrary);
public:
	static FString GetEngineVersion();

	static void ExecuteConsoleCommand(UObject* WorldContextObject, const FString& Command, APlayerController* SpecificPlayer);

	static FTimerHandle K2_SetTimer(UObject* Object, const FString& FunctionName, float Time, bool bLooping);

	static bool LineTraceSingle(
		UObject* WorldContextObject,
		const FVector& Start,
		const FVector& End,
		uint8 TraceChannel,
		bool bTraceComplex,
		TArray<AActor*>& ActorsToIgnore,
		uint8 DrawDebugType,
		FHitResult* OutHit,
		bool bIgnoreSelf,
		const FLinearColor& TraceColor = FLinearColor::Red,
		const FLinearColor& TraceHitColor = FLinearColor::Green,
		float DrawTime = 5.0f
	);

	static bool IsDedicatedServer(const UObject* WorldContextObject);

	static bool IsServer(const UObject* WorldContextObject);

	static bool IsStandalone(const UObject* WorldContextObject);
};