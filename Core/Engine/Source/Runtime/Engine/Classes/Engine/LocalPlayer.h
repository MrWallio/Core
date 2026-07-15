#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Player.h"

class AActor;
class FSceneView;
class UGameInstance;
class UGameViewportClient;
class ULocalPlayer;
struct FMinimalViewInfo;
struct FSceneViewProjectionData;

class ULocalPlayer : public UPlayer {
public:
	DefineUnrealClass(ULocalPlayer);

	DefineUProperty(UGameViewportClient*, ViewportClient);
public:
	FORCEINLINE UGameViewportClient* GetViewportClient() const { return ViewportClient; }
};