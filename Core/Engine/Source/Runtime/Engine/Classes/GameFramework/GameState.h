#pragma once
#include "pch.h"

#include "GameStateBase.h"

class AGameState : public AGameStateBase {
public:
	DefineUnrealClass(AGameState);

	DefineUProperty(FName, MatchState);
	DefineUProperty(int32, ElapsedTime);
public:
	FORCEINLINE FName GetMatchState() const { return MatchState; }
};