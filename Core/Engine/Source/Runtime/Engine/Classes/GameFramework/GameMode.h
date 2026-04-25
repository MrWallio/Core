#pragma once
#include "pch.h"

#include "GameModeBase.h"
#include "../Kismet/KismetStringLibrary.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

class APlayerState;
class ULocalMessage;
class UNetDriver;
class AController;

struct FVector;
struct FRotator;

namespace MatchState
{
	extern const FName EnteringMap;			// We are entering this map, actors are not yet ticking
	extern const FName WaitingToStart;		// Actors are ticking, but the match has not yet started
	extern const FName InProgress;			// Normal gameplay is occurring. Specific games will have their own state machine inside this state
	extern const FName WaitingPostMatch;		// Match has ended so we aren't accepting new players, but actors are still ticking
	extern const FName LeavingMap;			// We are transitioning out of the map to another location
	extern const FName Aborted;				// Match has failed due to network issues or other problems, cannot continue
}

class AGameMode : public AGameModeBase {
public:
	DefineUnrealClass(AGameMode);
public:
	DefineUProperty(FName, MatchState);

	DefineUProperty(int32, NumSpectators);

	DefineUProperty(int32, NumPlayers);

	DefineUProperty(int32, NumTravellingPlayers);
public:
	void AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC);

	bool FindInactivePlayer(APlayerController* PC);

	void Broadcast(AActor* Sender, const FString& Msg, FName Type = UKismetStringLibrary::Conv_StringToName(L"None"));

	void BroadcastLocalized(AActor* Sender, TSubclassOf<ULocalMessage> Message, int32 Switch = 0, APlayerState* RelatedPlayerState_1 = NULL, APlayerState* RelatedPlayerState_2 = NULL, UObject* OptionalObject = NULL);

	void StartMatch();

	void EndMatch();

	FString GetNetworkNumber();

	void HandleMatchIsWaitingToStart();

	void HandleMatchHasStarted();

	void HandleMatchHasEnded();

	void HandleSeamlessTravelPlayer(AController*& C);

	void HandleStartingNewPlayer(APlayerController* NewPlayer);

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	void InitSeamlessTravelPlayer(AController* NewController);

	bool IsHandlingReplays();

	void OnMatchStateSet();

	bool PlayerCanRestart(APlayerController* Player);

	void PostSeamlessTravel();

	bool ReadyToStartMatch();

	void RestartGame();

	void Say(const FString& Msg);

	void SendPlayer(APlayerController* aPlayer, const FString& URL);

	void SetMatchState(FName NewState);

	void SetSeamlessTravelViewTarget(APlayerController* PC);

	void StartPlay();

	FName GetMatchState() const { return MatchState; }

	bool IsMatchInProgress() const;

	bool HasMatchEnded() const;

	void AbortMatch();

	bool ReadyToEndMatch();

	void PlayerSwitchedToSpectatorOnly(APlayerController* PC);

	void RemovePlayerControllerFromPlayerCount(APlayerController* PC);

	FString GetDefaultGameClassPath(const FString& MapName, const FString& Options, const FString& Portal) const;

	TSubclassOf<AGameMode> GetGameModeClass(const FString& MapName, const FString& Options, const FString& Portal) const;

	void OverridePlayerState(APlayerController* PC, APlayerState* OldPlayerState);

	void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver);

	bool HasMatchStarted() const;

	void PostLogin(APlayerController* NewPlayer);

	void Logout(AController* Exiting);

	int32 GetNumPlayers();

	int32 GetNumSpectators();

	bool CanServerTravel(const FString& URL, bool bAbsolute);

	void StartToLeaveMap();
};