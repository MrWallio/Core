#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

void AGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	void (*InitGameInternal)(AGameModeBase*, const FString&, const FString&, FString&) = decltype(InitGameInternal)(ImageBase + Finder::FindAGameModeBase_InitGame());
	InitGameInternal(this, MapName, Options, ErrorMessage);
}

void AGameModeBase::InitGameState()
{
	void (*InitGameStateInternal)(AGameModeBase*) = decltype(InitGameStateInternal)(ImageBase + Finder::FindAGameModeBase_InitGameState());
	InitGameStateInternal(this);
}

bool AGameModeBase::CanServerTravel(const FString& FURL, bool bAbsolute)
{
	bool (*CanServerTravelInternal)(AGameModeBase*, const FString&, bool) = decltype(CanServerTravelInternal)(ImageBase + Finder::FindAGameModeBase_CanServerTravel());
	return CanServerTravelInternal(this, FURL, bAbsolute);
}

bool AGameModeBase::MustSpectate(APlayerController* NewPlayerController) const
{
	if (!NewPlayerController || !NewPlayerController->PlayerState)
	{
		return false;
	}

	return NewPlayerController->PlayerState->IsOnlyASpectator();
}

APawn* AGameModeBase::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
	APawn* (*SpawnDefaultPawnForInternal)(AGameModeBase*, AController*, AActor*) = decltype(SpawnDefaultPawnForInternal)(ImageBase + Finder::FindAGameModeBase_SpawnDefaultPawnFor());
	return SpawnDefaultPawnForInternal(this, NewPlayer, StartSpot);
}

APawn* AGameModeBase::SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	APawn* (*SpawnDefaultPawnAtTransformInternal)(AGameModeBase*, AController*, const FTransform&) = decltype(SpawnDefaultPawnAtTransformInternal)(ImageBase + Finder::FindAGameModeBase_SpawnDefaultPawnAtTransform());
	return SpawnDefaultPawnAtTransformInternal(this, NewPlayer, SpawnTransform);
}

void AGameModeBase::RestartPlayer(AController* NewPlayer)
{
	void (*RestartPlayerInternal)(AGameModeBase*, AController*) = decltype(RestartPlayerInternal)(ImageBase + Finder::FindAGameModeBase_RestartPlayer());
	RestartPlayerInternal(this, NewPlayer);
}

void AGameModeBase::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	void (*FinishRestartPlayerInternal)(AGameModeBase*, AController*, const FRotator&) = decltype(FinishRestartPlayerInternal)(ImageBase + Finder::FindAGameModeBase_FinishRestartPlayer());
	FinishRestartPlayerInternal(this, NewPlayer, StartRotation);
}

void AGameModeBase::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	void (*HandleStartingNewPlayerInternal)(AGameModeBase*, APlayerController*) = decltype(HandleStartingNewPlayerInternal)(ImageBase + Finder::FindAGameModeBase_HandleStartingNewPlayer());
	HandleStartingNewPlayerInternal(this, NewPlayer);
}