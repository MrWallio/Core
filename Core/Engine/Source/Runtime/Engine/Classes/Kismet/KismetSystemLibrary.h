#pragma once
#include "pch.h"

#include "BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/TimerHandle.h"

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

	static bool IsDedicatedServer(const UObject* WorldContextObject);

	static bool IsServer(const UObject* WorldContextObject);

	static bool IsStandalone(const UObject* WorldContextObject);
};