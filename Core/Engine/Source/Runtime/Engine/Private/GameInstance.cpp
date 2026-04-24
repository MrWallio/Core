#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"

void UGameInstance::StartGameInstance()
{
	void (*StartGameInstanceInternal)(UGameInstance*) = decltype(StartGameInstanceInternal)(ImageBase + Finder::FindUGameInstance_StartGameInstance());
	StartGameInstanceInternal(this);
}

void UGameInstance::Shutdown()
{
	void (*ShutdownInternal)(UGameInstance*) = decltype(ShutdownInternal)(ImageBase + Finder::FindUGameInstance_Shutdown());
	ShutdownInternal(this);
}

APlayerController* UGameInstance::GetFirstLocalPlayerController(const UWorld* World) const
{
	APlayerController* (*GetFirstLocalPlayerControllerInternal)(const UGameInstance*, const UWorld*) = decltype(GetFirstLocalPlayerControllerInternal)(ImageBase + Finder::FindUGameInstance_GetFirstLocalPlayerController());
	return GetFirstLocalPlayerControllerInternal(this, World);
}

bool UGameInstance::ClientTravelToSession(int32 ControllerId, FName InSessionName)
{
	bool (*ClientTravelToSessionInternal)(UGameInstance*, int32, FName) = decltype(ClientTravelToSessionInternal)(ImageBase + Finder::FindUGameInstance_ClientTravelToSession());
	return ClientTravelToSessionInternal(this, ControllerId, InSessionName);
}

void UGameInstance::CleanupGameViewport()
{
	void (*CleanupGameViewportInternal)(UGameInstance*) = decltype(CleanupGameViewportInternal)(ImageBase + Finder::FindUGameInstance_CleanupGameViewport());
	CleanupGameViewportInternal(this);
}