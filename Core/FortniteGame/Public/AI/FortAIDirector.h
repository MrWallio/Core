#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class AFortAIDirector : public AActor {
public:
	DefineUnrealClass(AFortAIDirector);
public:
	void Activate();
};