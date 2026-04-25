#pragma once
#include "pch.h"

#include "FortGameStateZone.h"
#include "FortniteGame/Public/FortPlaylist/PlaylistPropertyArray.h"

class AFortAthenaMapInfo;

class AFortGameStateAthena : public AFortGameStateZone {
public:
	DefineUnrealClass(AFortGameStateAthena);

	DefineUProperty(AFortAthenaMapInfo*, MapInfo);

	DefineUProperty(FPlaylistPropertyArray, CurrentPlaylistInfo);
	DefineUProperty(int32, CurrentPlaylistId);
public:
	void OnRep_CurrentPlaylistInfo();
	void OnRep_CurrentPlaylistId();

	void OnPlaylistDataLoadCompleted();
	void UpdatePlaylistDependentData();

	void SetCurrentPlaylistId(int InPlaylistId);
};