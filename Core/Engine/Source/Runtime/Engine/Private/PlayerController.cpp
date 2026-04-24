#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

bool APlayerController::HasClientLoadedCurrentWorld()
{
	bool (*HasClientLoadedCurrentWorldInternal)(APlayerController*) = decltype(HasClientLoadedCurrentWorldInternal)(ImageBase + Finder::FindAPlayerController_HasClientLoadedCurrentWorld());
	return HasClientLoadedCurrentWorldInternal(this);
}

void APlayerController::ServerAcknowledgePossession(APawn* P)
{
	void (*ServerAcknowledgePossessionInternal)(APlayerController*, APawn*) = decltype(ServerAcknowledgePossessionInternal)(ImageBase + Finder::FindAPlayerController_ServerAcknowledgePossession());
	ServerAcknowledgePossessionInternal(this, P);
}

AActor* APlayerController::GetViewTarget() const
{
	AActor* (*GetViewTargetInternal)(const APlayerController*) = decltype(GetViewTargetInternal)(ImageBase + Finder::FindAPlayerController_GetViewTarget());
	return GetViewTargetInternal(this);
}

void APlayerController::SendClientAdjustment()
{
	void (*SendClientAdjustmentInternal)(APlayerController*) = decltype(SendClientAdjustmentInternal)(ImageBase + Finder::FindAPlayerController_SendClientAdjustment());
	SendClientAdjustmentInternal(this);
}