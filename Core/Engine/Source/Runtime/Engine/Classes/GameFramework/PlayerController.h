#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

class ACameraActor;
class AHUD;
class APawn;
class APlayerCameraManager;
class ASpectatorPawn;
class FDebugDisplayInfo;
class UActorChannel;
class UCheatManager;
class UGameViewportClient;
class UInterpTrackInstDirector;
class ULocalMessage;
class UNetConnection;
class UPlayer;
class UPrimitiveComponent;
struct FActiveHapticFeedbackEffect;
struct FCollisionQueryParams;

class APlayerController : public AController {
public:
	DefineUnrealClass(APlayerController);

	DefineUProperty(UCheatManager*, CheatManager);

	DefineUProperty(TSubclassOf<UCheatManager>, CheatClass);
public:
	static inline void (*ServerAcknowledgePossessionOG)(APlayerController* This, APawn* P);
	void ServerAcknowledgePossession(APawn* P);

	void SendClientAdjustment();

	static void Hook() {
		CreateVTableOriginal(APlayerController::GetDefaultObj(), APlayerController::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), (LPVOID*)&ServerAcknowledgePossessionOG);

		Log("Hooked APlayerController");
	}
};