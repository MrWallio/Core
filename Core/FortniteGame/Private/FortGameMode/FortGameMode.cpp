#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameMode.h"

APawn* AFortGameMode::SpawnDefaultPawnFor(AFortPlayerController* NewPlayer, AActor* StartSpot) {
	APawn* (*SpawnDefaultPawnForInternal)(AFortGameMode*, AFortPlayerController*, AActor*) = decltype(SpawnDefaultPawnForInternal)(ImageBase + Finder::FindAFortGameMode_SpawnDefaultPawnFor());
	return SpawnDefaultPawnForInternal(this, NewPlayer, StartSpot);
}