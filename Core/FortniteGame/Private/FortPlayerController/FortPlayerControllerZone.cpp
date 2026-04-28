#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"

void AFortPlayerControllerZone::ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P) {
	AFortPlayerStateZone* PlayerState = This->PlayerState ? This->PlayerState->Cast<AFortPlayerStateZone>() : nullptr;
	if (!PlayerState) {
		Log("ServerAcknowledgePossession: PlayerState is null or not AFortPlayerStateZone");
		return;
	}

	AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>();
	if (Version::Fortnite_Version <= 1.72) {
		if (FortPCAthena && FortPCAthena->StrongMyHero && FortPCAthena->StrongMyHero->CharacterParts.Num() > 0) {
			for (UCustomCharacterPart* CharacterPart : FortPCAthena->StrongMyHero->CharacterParts) {
				P->ServerChoosePart(CharacterPart, CharacterPart->CharacterPartType);
			}
		}
		else {
			P->ServerChoosePart((UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1"), EFortCustomPartType::Head);
			P->ServerChoosePart((UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01"), EFortCustomPartType::Body);

			Log("SpawnDefaultPawnFor: Applied default CharacterParts because FortPCAthena or StrongMyHero is null or no CharacterParts!");
		}

		PlayerState->OnRep_CharacterParts();
	}
	else {
		// ApplyCharacterCustomization
	}
	
	AFortPlayerController::ServerAcknowledgePossessionOG(This, P);
}

void AFortPlayerControllerZone::Hook() {
	HookEveryVTable(AFortPlayerControllerZone::StaticClass(), AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), ServerAcknowledgePossession, nullptr);
	
	Log("Hooked AFortPlayerControllerZone");
}