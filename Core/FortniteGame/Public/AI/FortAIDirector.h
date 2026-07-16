#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

class AFortAIDirector : public AActor {
public:
	DefineUnrealClass(AFortAIDirector);
public:
	void Activate();

	static AFortAIDirector* GetCurrent(UObject* WorldContextObject);

	static void Hook();
};