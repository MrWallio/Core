#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Actor.h"
#include "PlayerState.h"

class AController;
class APhysicsVolume;
class APlayerController;
class APlayerState;
class FDebugDisplayInfo;
class UCanvas;
class UDamageType;
class UInputComponent;
class UPawnMovementComponent;
class UPawnNoiseEmitterComponent;
class UPlayer;
class UPrimitiveComponent;

class APawn : public AActor {
public:
	DefineUnrealClass(APawn);

	DefineUProperty(AController*, Controller);
	DefineUProperty(APlayerState*, PlayerState);
	DefineUProperty(AController*, LastHitBy);
	DefineUProperty(float, BaseEyeHeight);
	DefineUProperty(uint8, RemoteViewPitch);
	DefineBitfieldUProperty(bUseControllerRotationPitch);
	DefineBitfieldUProperty(bUseControllerRotationYaw);
	DefineBitfieldUProperty(bUseControllerRotationRoll);
	DefineBitfieldUProperty(bCanAffectNavigationGeneration);
public:
	FORCEINLINE AController* GetController() const { return Controller; }

	FORCEINLINE APlayerState* GetPlayerState() const { return PlayerState; }
	template<class T>
	FORCEINLINE T* GetPlayerState() const { return Cast<T>(PlayerState); }

	FORCEINLINE bool IsPlayerControlled() const { return PlayerState && !PlayerState->bIsABot; }
	FORCEINLINE bool IsBotControlled() const { return PlayerState && PlayerState->bIsABot; }

	UPawnMovementComponent* GetMovementComponent() const;
public:
	void OnRep_PlayerState();
};
