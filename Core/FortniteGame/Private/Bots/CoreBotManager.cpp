#include "pch.h"
#include "FortniteGame/Public/Bots/CoreBotManager.h"

#include "FortniteGame/Public/FortGameMode/FortGameMode.h"
#include "FortniteGame/Public/FortPlayerStart/FortPlayerStartWarmup.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "FortniteGame/Public/AI/FortAIController.h"
#include "FortniteGame/Public/AI/FortAIPawn.h"

CoreBotManager& CoreBotManager::Get()
{
	static CoreBotManager Instance;
	return Instance;
}

AController* CoreBotManager::SpawnBotController(const FBotSpawnOptions& Options, AFortGameMode* GameMode, std::string& OutError) {
	UClass* ControllerClass = ResolveControllerClass(Options, GameMode, OutError);
	if (!ControllerClass) {
		return nullptr;
	}

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		OutError = "World is null";
		return nullptr;
	}

	AActor* SpawnedController = World->SpawnActor(ControllerClass);
	if (!SpawnedController) {
		OutError = "Failed to spawn controller of class " + ControllerClass->GetName().ToString();
		return nullptr;
	}

	AController* Controller = SpawnedController->Cast<AController>();
	if (!Controller) {
		OutError = "Spawned actor " + SpawnedController->GetName().ToString() + " is not an AController";
		SpawnedController->K2_DestroyActor();
		return nullptr;
	}

	return Controller;
}

AActor* CoreBotManager::ResolveSpawnPoint(const FBotSpawnOptions& Options)
{
	if (Options.SpawnPoint) {
		return Options.SpawnPoint;
	}

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return nullptr;
	}

	if (CachedPlayerStarts.Num() == 0) {
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerStartWarmup::StaticClass(), &CachedPlayerStarts);
	}

	if (CachedPlayerStarts.Num() == 0) {
		return nullptr;
	}

	if (PlayerStartIndex >= CachedPlayerStarts.Num()) {
		PlayerStartIndex = 0;
	}

	return CachedPlayerStarts[PlayerStartIndex++];
}

UClass* CoreBotManager::ResolvePawnClass(const FBotSpawnOptions& Options, AFortGameMode* GameMode, std::string& OutError)
{
	if (Options.PawnClassOverride) {
		return Options.PawnClassOverride;
	}

	if (Options.Kind == EBotKind::AI) {
		OutError = "AI bot needs a pawn class (pass CustomPawnClass=...)";
		return nullptr;
	}

	if (!GameMode) {
		OutError = "No game mode (cannot resolve DefaultPawnClass for a player bot)";
		return nullptr;
	}

	UClass* PawnClass = GameMode->DefaultPawnClass.Get();
	if (!PawnClass) {
		OutError = "No pawn class (DefaultPawnClass not set yet?)";
	}

	return PawnClass;
}

UClass* CoreBotManager::ResolveControllerClass(const FBotSpawnOptions& Options, AFortGameMode* GameMode, std::string& OutError) {
	if (Options.ControllerType == EBotControllerType::AI) {
		return AFortAIController::StaticClass();
	}

	if (Options.ControllerType == EBotControllerType::PlayerBot) {
		// in later versions we can use a different controller
		return AFortAIController::StaticClass();
	}

	if (!GameMode) {
		OutError = "No game mode (cannot resolve PlayerControllerClass for a native bot)";
		return nullptr;
	}

	UClass* PlayerControllerClass = GameMode->PlayerControllerClass.Get();
	if (!PlayerControllerClass) {
		OutError = "No PlayerController class (PlayerControllerClass not set yet?)";
	}

	return PlayerControllerClass;
}

AFortGameMode* CoreBotManager::ResolveGameMode(UWorld* World) {
	if (!World || !World->AuthorityGameMode) {
		return nullptr;
	}

	return World->AuthorityGameMode->Cast<AFortGameMode>();
}

void CoreBotManager::ComputeSpawnTransform(const FBotSpawnOptions& Options, AActor* SpawnPoint, FVector& OutLocation, FRotator& OutRotation) {
	OutLocation = SpawnPoint->K2_GetActorLocation();
	OutRotation = SpawnPoint->K2_GetActorRotation();

	if (Options.SpawnPoint) {
		const int32 RingIndex = (int32)Bots.size();
		const float Angle = (RingIndex % 8) * 0.785398f;
		const float Ring = 300.0f + 150.0f * (RingIndex / 8);

		OutLocation.X += cosf(Angle) * Ring;
		OutLocation.Y += sinf(Angle) * Ring;
		OutLocation.Z += 50.0f;
	}
}

APawn* CoreBotManager::SpawnBotPawn(UClass* PawnClass, const FVector& Location, const FRotator& Rotation, std::string& OutError) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		OutError = "World is null";
		return nullptr;
	}

	AActor* PawnActor = World->SpawnActor(PawnClass, Location, Rotation);
	if (!PawnActor) {
		OutError = "SpawnActor failed for pawn class " + PawnClass->GetName().ToString();
		return nullptr;
	}

	APawn* Pawn = PawnActor->Cast<APawn>();
	if (!Pawn) {
		OutError = "Spawned actor " + PawnActor->GetName().ToString() + " is not an APawn";
		PawnActor->K2_DestroyActor();
		return nullptr;
	}

	return Pawn;
}

APawn* CoreBotManager::RegisterBot(const FBotSpawnOptions& Options, AController* Controller, APawn* Pawn) {
	FBotEntry Entry;
	Entry.BotId = NextBotId++;
	Entry.Controller = Controller;
	Entry.Pawn = Pawn;
	Entry.BehaviorTree = Options.BehaviorTree;
	Entry.Kind = Options.Kind;
	Entry.BrainMode = Options.BrainMode;
	Entry.ControllerType = Options.ControllerType;
	Bots.push_back(Entry);

	Log("CoreBotManager: spawned " + std::string(Options.Kind == EBotKind::PlayerBot ? "player" : "AI")
		+ " bot " + std::to_string(Entry.BotId) + " (" + Pawn->GetName().ToString()
		+ ", controller " + Controller->GetName().ToString() + ")");

	return Pawn;
}

APawn* CoreBotManager::SpawnBot(const FBotSpawnOptions& Options, std::string& OutError)
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		OutError = "World is null";
		return nullptr;
	}

	AFortGameMode* GameMode = ResolveGameMode(World);

	AActor* SpawnPoint = ResolveSpawnPoint(Options);
	if (!SpawnPoint) {
		OutError = "No spawn point (no AFortPlayerStartWarmup found and none given)";
		return nullptr;
	}

	UClass* PawnClass = ResolvePawnClass(Options, GameMode, OutError);
	if (!PawnClass) {
		return nullptr;
	}

	FVector SpawnLocation;
	FRotator SpawnRotation;
	ComputeSpawnTransform(Options, SpawnPoint, SpawnLocation, SpawnRotation);

	AController* Controller = SpawnBotController(Options, GameMode, OutError);
	if (!Controller) {
		return nullptr;
	}

	APawn* Pawn = SpawnBotPawn(PawnClass, SpawnLocation, SpawnRotation, OutError);
	if (!Pawn) {
		Controller->K2_DestroyActor();
		return nullptr;
	}

	return RegisterBot(Options, Controller, Pawn);
}

int32 CoreBotManager::DespawnAll()
{
	int32 Removed = 0;

	for (FBotEntry& Bot : Bots) {
		if (Bot.Pawn) {
			Bot.Pawn->K2_DestroyActor();
			Removed++;
		}

		if (Bot.Controller)
			Bot.Controller->K2_DestroyActor();
	}

	Bots.clear();
	return Removed;
}

void CoreBotManager::PruneDeadEntries()
{
	for (auto It = Bots.begin(); It != Bots.end();) {
		if (!It->Pawn || It->Pawn->IsPendingKillPending())
			It = Bots.erase(It);
		else
			++It;
	}
}

void CoreBotManager::Reset()
{
	Bots.clear();
	NextBotId = 1;
	CachedPlayerStarts = TArray<AActor*>();
	PlayerStartIndex = 0;
}

void CoreBotManager::Tick(float DeltaSeconds)
{
	
}

FBotEntry* CoreBotManager::FindByPawn(APawn* Pawn)
{
	if (!Pawn)
		return nullptr;

	for (FBotEntry& Bot : Bots) {
		if (Bot.Pawn == Pawn)
			return &Bot;
	}

	return nullptr;
}
