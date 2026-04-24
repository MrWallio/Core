#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Player.h"

APlayerController* UPlayer::GetPlayerController(UWorld* InWorld) const
{
	APlayerController* (*GetPlayerControllerInternal)(const UPlayer*, UWorld*) = decltype(GetPlayerControllerInternal)(ImageBase + Finder::FindUPlayer_GetPlayerController());
	return GetPlayerControllerInternal(this, InWorld);
}

void UPlayer::SwitchController(class APlayerController* PC)
{
	void (*SwitchControllerInternal)(UPlayer*, APlayerController*) = decltype(SwitchControllerInternal)(ImageBase + Finder::FindUPlayer_SwitchController());
	SwitchControllerInternal(this, PC);
}