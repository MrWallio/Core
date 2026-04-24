#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

class ULocalPlayer;
class UOnlineSession;
class AGameModeBase;
class APlayerController;
class UWorld;

class UGameInstance : public UObject {
public:
	DefineUnrealClass(UGameInstance);

	DefineUProperty(TArray<ULocalPlayer*>, LocalPlayers);
	DefineUProperty(UOnlineSession*, OnlineSession);
	DefineUProperty(TArray<UObject>, ReferencedObjects);
public:
	/** Starts the GameInstance state machine running */
	void StartGameInstance();

	void Shutdown();

	APlayerController* GetFirstLocalPlayerController(const UWorld* World = nullptr) const;

	/**
	 * Helper function for traveling to a session that has already been joined via the online platform
	 * Grabs the URL from the session info and travels
	 *
	 * @param ControllerId controller initiating the request
	 * @param InSessionName name of session to travel to
	 *
	 * @return true if able or attempting to travel, false otherwise
	 */
	bool ClientTravelToSession(int32 ControllerId, FName InSessionName);

	void CleanupGameViewport();
};