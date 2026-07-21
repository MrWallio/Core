#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Player.h"

APlayerController* UPlayer::GetPlayerController(UWorld* InWorld) const
{
	return PlayerController;
}