#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class APlayerController;
class UCanvas;
class UFont;

class AHUD : public AActor {
public:
	DefineUnrealClass(AHUD);

	DefineUProperty(APlayerController*, PlayerOwner);
	DefineBitfieldUProperty(bShowHUD);
public:
	FORCEINLINE APlayerController* GetOwningPlayerController() const { return PlayerOwner; }
};
