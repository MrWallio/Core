#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameMode.h"

#include "FortniteGame/Public/AI/FortAIController.h"
#include "FortniteGame/Public/AI/FortAIPawn.h"
#include "FortniteGame/Public/FortPlayerStart/FortPlayerStartWarmup.h"

bool AFortGameMode::SpawnPlayerBot(AActor* SpawnPoint)
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortGameMode::SpawnPlayerBot: World is null!");
		return false;
	}

	static TArray<AActor*> PlayerStarts;
	if (PlayerStarts.Num() == 0) {
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerStartWarmup::StaticClass(), &PlayerStarts);
	}
	
	int32 MaxSpawnLocations = PlayerStarts.Num();
	static int32 BotSpawnLocationIndex = 0;

	if (Version::Fortnite_Version <= 1.72) {
		if (!SpawnPoint && PlayerStarts.Num() > 0) {
			if (BotSpawnLocationIndex >= MaxSpawnLocations) {
				BotSpawnLocationIndex = 0;
			}
			SpawnPoint = PlayerStarts.IsValidIndex(BotSpawnLocationIndex) ? PlayerStarts[BotSpawnLocationIndex] : nullptr;
			BotSpawnLocationIndex++;
		}

		if (!SpawnPoint) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to find spawn point!");
			return false;
		}

		FVector SpawnLocation = SpawnPoint->K2_GetActorLocation();
		FRotator SpawnRotation = SpawnPoint->K2_GetActorRotation();

		APlayerController* BotController = SpawnPlayerController(
			ROLE_SimulatedProxy,
			SpawnLocation,
			SpawnRotation
		);
		if (!BotController) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to spawn BotController!");
			return false;
		}

		if (!BotController->PlayerState) {
			BotController->InitPlayerState();
		}

		if (!BotController->PlayerState) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to initialize BotController's PlayerState!");
			return false;
		}

		RestartPlayerAtTransform(BotController, SpawnPoint->GetTransform());
		if (!BotController->Pawn) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to spawn bot pawn!");
			return false;
		}

		return true;
	}

	return false;
}