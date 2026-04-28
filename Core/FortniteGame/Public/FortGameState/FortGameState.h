#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameState.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "FortniteGame/Public/FortWorld/AdditionalLevelStreamed.h"

class FUniqueNetIdRepl;
class UFortMcpProfileCampaign;
class UFortHero;
class IAbilitySystemInterface;

class AFortGameState : public AGameState {
public:
	DefineUnrealClass(AFortGameState);

	DefineUProperty(TArray<FAdditionalLevelStreamed>, AdditionalPlaylistLevelsStreamed);
public:
	void OnRep_AdditionalPlaylistLevelsStreamed();
	void OnFinishedStreamingAdditionalPlaylistLevel();

	static void Hook();
};