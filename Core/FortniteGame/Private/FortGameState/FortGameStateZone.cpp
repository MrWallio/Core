#include "pch.h"
#include "FortniteGame/Public/FortGameState/FortGameStateZone.h"

#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"

void AFortGameStateZone::ApplyHomebaseEffectsOnPlayerSetup(AFortGameStateZone* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp)
{
	UFortHeroType* ItemDefinition = StaticLoadObject<UFortHeroType>("/Game/Athena/Heroes/HID_Commando_Athena_01.HID_Commando_Athena_01");

	static TArray<UObject*> HeroTypes = FUObjectArray::GetObjectsOfClass(UFortHeroType::StaticClass(), "Athena");
	if (HeroTypes.Num() > 0)
	{
		ItemDefinition = (UFortHeroType*)HeroTypes[UKismetMathLibrary::RandomIntegerInRange(0, HeroTypes.Num() - 1)];
	}

	if (ItemDefinition)
	{
		Hero->ItemDefinition = ItemDefinition;
		Log("AFortGameStateZone::ApplyHomebaseEffectsOnPlayerSetup: Set hero item definition to " + ItemDefinition->GetName().ToString());
	}

	ApplyHomebaseEffectsOnPlayerSetupOG(This, SourceAccountID, McpProfile, AbilityObject, Hero, bApplyTeamEffect, bApplyTeamEffectToOtherPlayers, bIgnoreStatClamp);
}

void AFortGameStateZone::Hook() {
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup()), ApplyHomebaseEffectsOnPlayerSetup, (LPVOID*)&ApplyHomebaseEffectsOnPlayerSetupOG);

	Log("Hooked AFortGameStateZone");
}