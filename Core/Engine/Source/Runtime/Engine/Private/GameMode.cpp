#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameMode.h"

namespace MatchState
{
	const FName EnteringMap = UKismetStringLibrary::Conv_StringToName(TEXT("EnteringMap"));
	const FName WaitingToStart = UKismetStringLibrary::Conv_StringToName(TEXT("WaitingToStart"));
	const FName InProgress = UKismetStringLibrary::Conv_StringToName(TEXT("InProgress"));
	const FName WaitingPostMatch = UKismetStringLibrary::Conv_StringToName(TEXT("WaitingPostMatch"));
	const FName LeavingMap = UKismetStringLibrary::Conv_StringToName(TEXT("LeavingMap"));
	const FName Aborted = UKismetStringLibrary::Conv_StringToName(TEXT("Aborted"));
}

void AGameMode::AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC)
{
	void (*AddInactivePlayerInternal)(AGameMode*, APlayerState*, APlayerController*) = decltype(AddInactivePlayerInternal)(ImageBase + Finder::FindAGameMode_AddInactivePlayer());
	AddInactivePlayerInternal(this, PlayerState, PC);
}

bool AGameMode::FindInactivePlayer(APlayerController* PC)
{
	bool (*FindInactivePlayerInternal)(AGameMode*, APlayerController*) = decltype(FindInactivePlayerInternal)(ImageBase + Finder::FindAGameMode_FindInactivePlayer());
	return FindInactivePlayerInternal(this, PC);
}

void AGameMode::Broadcast(AActor* Sender, const FString& Msg, FName Type)
{
	void (*BroadcastInternal)(AGameMode*, AActor*, const FString&, FName) = decltype(BroadcastInternal)(ImageBase + Finder::FindAGameMode_Broadcast());
	BroadcastInternal(this, Sender, Msg, Type);
}

void AGameMode::BroadcastLocalized(AActor* Sender, TSubclassOf<ULocalMessage> Message, int32 Switch, APlayerState* RelatedPlayerState_1, APlayerState* RelatedPlayerState_2, UObject* OptionalObject)
{
	void (*BroadcastLocalizedInternal)(AGameMode*, AActor*, TSubclassOf<ULocalMessage>, int32, APlayerState*, APlayerState*, UObject*) = decltype(BroadcastLocalizedInternal)(ImageBase + Finder::FindAGameMode_BroadcastLocalized());
	BroadcastLocalizedInternal(this, Sender, Message, Switch, RelatedPlayerState_1, RelatedPlayerState_2, OptionalObject);
}

void AGameMode::StartMatch()
{
	void (*StartMatchInternal)(AGameMode*) = decltype(StartMatchInternal)(ImageBase + Finder::FindAGameMode_StartMatch());
	StartMatchInternal(this);
}

void AGameMode::EndMatch()
{
	void (*EndMatchInternal)(AGameMode*) = decltype(EndMatchInternal)(ImageBase + Finder::FindAGameMode_EndMatch());
	EndMatchInternal(this);
}

FString AGameMode::GetNetworkNumber()
{
	FString(*GetNetworkNumberInternal)(AGameMode*) = decltype(GetNetworkNumberInternal)(ImageBase + Finder::FindAGameMode_GetNetworkNumber());
	return GetNetworkNumberInternal(this);
}

void AGameMode::HandleMatchHasEnded()
{
	void (*HandleMatchHasEndedInternal)(AGameMode*) = decltype(HandleMatchHasEndedInternal)(ImageBase + Finder::FindAGameMode_HandleMatchHasEnded());
	HandleMatchHasEndedInternal(this);
}

void AGameMode::HandleMatchHasStarted()
{
	void (*HandleMatchHasStartedInternal)(AGameMode*) = decltype(HandleMatchHasStartedInternal)(ImageBase + Finder::FindAGameMode_HandleMatchHasStarted());
	HandleMatchHasStartedInternal(this);
}

void AGameMode::HandleMatchIsWaitingToStart()
{
	void (*HandleMatchIsWaitingToStartInternal)(AGameMode*) = decltype(HandleMatchIsWaitingToStartInternal)(ImageBase + Finder::FindAGameMode_HandleMatchIsWaitingToStart());
	HandleMatchIsWaitingToStartInternal(this);
}

void AGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	void (*HandleSeamlessTravelPlayerInternal)(AGameMode*, AController*&) = decltype(HandleSeamlessTravelPlayerInternal)(ImageBase + Finder::FindAGameMode_HandleSeamlessTravelPlayer());
	HandleSeamlessTravelPlayerInternal(this, C);
}

void AGameMode::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	void (*HandleStartingNewPlayerInternal)(AGameMode*, APlayerController*) = decltype(HandleStartingNewPlayerInternal)(ImageBase + Finder::FindAGameMode_HandleStartingNewPlayer());
	HandleStartingNewPlayerInternal(this, NewPlayer);
}

void AGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	void (*InitGameInternal)(AGameMode*, const FString&, const FString&, FString&) = decltype(InitGameInternal)(ImageBase + Finder::FindAGameMode_InitGame());
	InitGameInternal(this, MapName, Options, ErrorMessage);
}

void AGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	void (*InitSeamlessTravelPlayerInternal)(AGameMode*, AController*) = decltype(InitSeamlessTravelPlayerInternal)(ImageBase + Finder::FindAGameMode_InitSeamlessTravelPlayer());
	InitSeamlessTravelPlayerInternal(this, NewController);
}

bool AGameMode::IsHandlingReplays()
{
	bool (*IsHandlingReplaysInternal)(AGameMode*) = decltype(IsHandlingReplaysInternal)(ImageBase + Finder::FindAGameMode_IsHandlingReplays());
	return IsHandlingReplaysInternal(this);
}

void AGameMode::OnMatchStateSet()
{
	void (*OnMatchStateSetInternal)(AGameMode*) = decltype(OnMatchStateSetInternal)(ImageBase + Finder::FindAGameMode_OnMatchStateSet());
	OnMatchStateSetInternal(this);
}

bool AGameMode::PlayerCanRestart(APlayerController* Player)
{
	bool (*PlayerCanRestartInternal)(AGameMode*, APlayerController*) = decltype(PlayerCanRestartInternal)(ImageBase + Finder::FindAGameMode_PlayerCanRestart());
	return PlayerCanRestartInternal(this, Player);
}

void AGameMode::PostSeamlessTravel()
{
	void (*PostSeamlessTravelInternal)(AGameMode*) = decltype(PostSeamlessTravelInternal)(ImageBase + Finder::FindAGameMode_PostSeamlessTravel());
	PostSeamlessTravelInternal(this);
}

bool AGameMode::ReadyToStartMatch()
{
	bool (*ReadyToStartMatchInternal)(AGameMode*) = decltype(ReadyToStartMatchInternal)(ImageBase + Finder::FindAGameMode_ReadyToStartMatch());
	return ReadyToStartMatchInternal(this);
}

void AGameMode::RestartGame()
{
	void (*RestartGameInternal)(AGameMode*) = decltype(RestartGameInternal)(ImageBase + Finder::FindAGameMode_RestartGame());
	RestartGameInternal(this);
}

void AGameMode::Say(const FString& Msg)
{
	void (*SayInternal)(AGameMode*, const FString&) = decltype(SayInternal)(ImageBase + Finder::FindAGameMode_Say());
	SayInternal(this, Msg);
}

void AGameMode::SendPlayer(APlayerController* aPlayer, const FString& FURL)
{
	void (*SendPlayerInternal)(AGameMode*, APlayerController*, const FString&) = decltype(SendPlayerInternal)(ImageBase + Finder::FindAGameMode_SendPlayer());
	SendPlayerInternal(this, aPlayer, FURL);
}

void AGameMode::SetMatchState(FName NewState)
{
	void (*SetMatchStateInternal)(AGameMode*, FName) = decltype(SetMatchStateInternal)(ImageBase + Finder::FindAGameMode_SetMatchState());
	SetMatchStateInternal(this, NewState);
}

void AGameMode::SetSeamlessTravelViewTarget(APlayerController* PC)
{
	void (*SetSeamlessTravelViewTargetInternal)(AGameMode*, APlayerController*) = decltype(SetSeamlessTravelViewTargetInternal)(ImageBase + Finder::FindAGameMode_SetSeamlessTravelViewTarget());
	SetSeamlessTravelViewTargetInternal(this, PC);
}

void AGameMode::StartPlay()
{
	void (*StartPlayInternal)(AGameMode*) = decltype(StartPlayInternal)(ImageBase + Finder::FindAGameMode_StartPlay());
	StartPlayInternal(this);
}

bool AGameMode::IsMatchInProgress() const
{
	if (GetMatchState() == MatchState::InProgress)
	{
		return true;
	}

	return false;
}

bool AGameMode::HasMatchEnded() const
{
	if (GetMatchState() == MatchState::WaitingPostMatch || GetMatchState() == MatchState::LeavingMap)
	{
		return true;
	}

	return false;
}

void AGameMode::AbortMatch()
{
	SetMatchState(MatchState::Aborted);
}

void AGameMode::PlayerSwitchedToSpectatorOnly(APlayerController* PC)
{
	RemovePlayerControllerFromPlayerCount(PC);
	NumSpectators++;
}

void AGameMode::RemovePlayerControllerFromPlayerCount(APlayerController* PC)
{
	if (PC)
	{
		if (MustSpectate(PC))
		{
			NumSpectators--;
		}
		else
		{
			if (UWorld::GetWorld()->IsInSeamlessTravel() || PC->HasClientLoadedCurrentWorld())
			{
				NumPlayers--;
			}
			else
			{
				NumTravellingPlayers--;
			}
		}
	}
}

FString AGameMode::GetDefaultGameClassPath(const FString& MapName, const FString& Options, const FString& Portal) const
{
	// This is called on the CDO
	return GetClass()->GetPathName();
}

TSubclassOf<AGameMode> AGameMode::GetGameModeClass(const FString& MapName, const FString& Options, const FString& Portal) const
{
	// This is called on the CDO
	return GetClass();
}

void AGameMode::OverridePlayerState(APlayerController* PC, APlayerState* OldPlayerState)
{
	void (*OverridePlayerStateInternal)(AGameMode*, APlayerController*, APlayerState*) = decltype(OverridePlayerStateInternal)(ImageBase + Finder::FindAGameMode_OverridePlayerState());
	OverridePlayerStateInternal(this, PC, OldPlayerState);
}

void AGameMode::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	AbortMatch();
}

bool AGameMode::HasMatchStarted() const
{
	if (GetMatchState() == MatchState::EnteringMap || GetMatchState() == MatchState::WaitingToStart)
	{
		return false;
	}

	return true;
}

void AGameMode::PostLogin(APlayerController* NewPlayer)
{
	void (*PostLoginInternal)(AGameMode*, APlayerController*) = decltype(PostLoginInternal)(ImageBase + Finder::FindAGameMode_PostLogin());
	PostLoginInternal(this, NewPlayer);
}

void AGameMode::Logout(AController* Exiting)
{
	void (*LogoutInternal)(AGameMode*, AController*) = decltype(LogoutInternal)(ImageBase + Finder::FindAGameMode_Logout());
	LogoutInternal(this, Exiting);
}

int32 AGameMode::GetNumPlayers()
{
	return NumPlayers + NumTravellingPlayers;
}

int32 AGameMode::GetNumSpectators()
{
	return NumSpectators;
}

bool AGameMode::CanServerTravel(const FString& URL, bool bAbsolute)
{
	bool (*CanServerTravelInternal)(AGameMode*, const FString&, bool) = decltype(CanServerTravelInternal)(ImageBase + Finder::FindAGameMode_CanServerTravel());
	return CanServerTravelInternal(this, URL, bAbsolute);
}

void AGameMode::StartToLeaveMap()
{
	SetMatchState(MatchState::LeavingMap);
}