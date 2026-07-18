#pragma once
#include "pch.h"

#include <vector>
#include <string>

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

class APlayerState;

enum class EBotBrainMode : uint8 {
	None,
	CoreTick,
	BehaviorTree
};

enum class EBotControllerType : uint8 {
	PlayerBot,
	AI,
	Native
};

enum class EBotKind : uint8 {
	PlayerBot,
	AI
};

struct FBotSpawnOptions {
	AActor* SpawnPoint = nullptr;
	UClass* PawnClassOverride = nullptr;
	UObject* BehaviorTree = nullptr;
	float Scale = 1.0f;
	float Health = -1.0f;
	float MaxHealth = -1.0f;
	EBotKind Kind = EBotKind::PlayerBot;
	EBotBrainMode BrainMode = EBotBrainMode::None;
	EBotControllerType ControllerType = EBotControllerType::AI;
};

struct FBotEntry {
	int32 BotId = 0;
	AController* Controller = nullptr;
	APawn* Pawn = nullptr;
	APlayerState* PlayerState = nullptr;
	UObject* BehaviorTree = nullptr;
	EBotKind Kind = EBotKind::PlayerBot;
	EBotBrainMode BrainMode = EBotBrainMode::None;
	EBotControllerType ControllerType = EBotControllerType::AI;
};

class CoreBotManager {
public:
	static CoreBotManager& Get();

	APawn* SpawnBot(const FBotSpawnOptions& Options, std::string& OutError);

	int32 DespawnAll();
	void PruneDeadEntries();
	void Reset();
	void Tick(float DeltaSeconds);

	FBotEntry* FindByPawn(APawn* Pawn);
	int32 NumBots() const { return (int32)Bots.size(); }

private:
	AController* SpawnBotController(const FBotSpawnOptions& Options, class AFortGameMode* GameMode, std::string& OutError);
	APawn* SpawnBotPawn(UClass* PawnClass, const FVector& Location, const FRotator& Rotation, std::string& OutError);
	APawn* RegisterBot(const FBotSpawnOptions& Options, int32 BotId, AController* Controller, APawn* Pawn, APlayerState* PlayerState);
	bool PossessBot(AController* Controller, APawn* Pawn, std::string& OutError);
	APlayerState* InitBotPlayerState(AController* Controller, AFortGameMode* GameMode, int32 BotId, std::string& OutError);
	void ApplyBotCosmetics(APawn* Pawn);

	class AFortGameMode* ResolveGameMode(UWorld* World);
	AActor* ResolveSpawnPoint(const FBotSpawnOptions& Options);
	UClass* ResolvePawnClass(const FBotSpawnOptions& Options, class AFortGameMode* GameMode, std::string& OutError);
	UClass* ResolveControllerClass(const FBotSpawnOptions& Options, class AFortGameMode* GameMode, std::string& OutError);
	void ComputeSpawnTransform(const FBotSpawnOptions& Options, AActor* SpawnPoint, FVector& OutLocation, FRotator& OutRotation);

	std::vector<FBotEntry> Bots;
	int32 NextBotId = 1;

	TArray<AActor*> CachedPlayerStarts;
	int32 PlayerStartIndex = 0;
};
