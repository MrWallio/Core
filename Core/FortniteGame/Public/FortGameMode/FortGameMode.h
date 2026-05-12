#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

class AFortPlayerController;

class AFortGameMode : public AGameMode {
public:
	DefineUnrealClass(AFortGameMode);

	DefineBitfieldUProperty(bWorldIsReady);

	DefineUProperty(int32, CurrentPlaylistId);
	DefineUProperty(FName, CurrentPlaylistName);
public:
	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameMode* This, AFortPlayerController* NewPlayer, AActor* StartSpot);

	bool SpawnPlayerBot(AActor* SpawnPoint = nullptr);

	static void Hook() {
		CreateVTableOriginal(AFortGameMode::GetDefaultObj(), AFortGameMode::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"), (LPVOID*)&SpawnDefaultPawnForOG);
	}
};