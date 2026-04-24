#pragma once
#include "pch.h"

#include "BlueprintFunctionLibrary.h"

class AActor;
class ACameraActor;
class APlayerController;
class UPrimitiveComponent;
class USceneComponent;

class UKismetSystemLibrary : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UKismetSystemLibrary);
public:
	// Engine build number, for displaying to end users.
	static FString GetEngineVersion();

	static void ExecuteConsoleCommand(UObject* WorldContextObject, const FString& Command, APlayerController* SpecificPlayer);
};