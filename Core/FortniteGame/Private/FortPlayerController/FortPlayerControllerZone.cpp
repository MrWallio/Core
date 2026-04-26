#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"

void AFortPlayerControllerZone::ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P) {
	AFortPlayerStateZone* PlayerState = This->PlayerState ? This->PlayerState->Cast<AFortPlayerStateZone>() : nullptr;
	if (!PlayerState) {
		Log("ServerAcknowledgePossession: PlayerState is null or not AFortPlayerStateZone");
		return;
	}

	if (Version::Fortnite_Version <= 1.72) {
		P->ServerChoosePart((UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1"), EFortCustomPartType::Head);
		P->ServerChoosePart((UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01"), EFortCustomPartType::Body);

		PlayerState->OnRep_CharacterParts();
	}
	else {
		// ApplyCharacterCustomization
	}
	
	((AFortPlayerController*)This)->ServerAcknowledgePossession(P);
}

void AFortPlayerControllerZone::Hook() {
	HookVTable(AFortPlayerControllerAthena::GetDefaultObj(), AFortPlayerControllerAthena::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), ServerAcknowledgePossession, nullptr);
	
	Log("Hooked AFortPlayerControllerZone");
}