#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

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

void AFortPlayerControllerZone::OnReadyToStartMatch(AFortPlayerControllerZone* This) {
	OnReadyToStartMatchOG(This);

	AFortGameModeZone* GameMode = This->GetWorld()->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (GameMode) {
		if (GameMode->StartingItems.Num() > 0)
		{
			Log("HandleStartingNewPlayer: Processing StartingItems for new player. Count: " + std::to_string(GameMode->StartingItems.Num()));
			for (int i = 0; i < GameMode->StartingItems.Num(); i++)
			{
				auto& StartingItem = GameMode->StartingItems.GetWithSize(i, FItemAndCount::GetSize());

				Log(std::format("StartingItem {}: Item={}, Count={}", i, StartingItem.Item ? StartingItem.Item->GetFName().ToString().ToString() : "None", StartingItem.Count));
				if (StartingItem.Count)
					This->WorldInventory->AddItem(StartingItem.Item, StartingItem.Count);
			}
		}
		else {
			Log(" Warning: No StartingItems found!");
		}
	}
}

void AFortPlayerControllerZone::Hook() {
	HookEveryVTable(AFortPlayerControllerZone::StaticClass(), AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), ServerAcknowledgePossession, nullptr);
	
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerControllerZone_OnReadyToStartMatch()), OnReadyToStartMatch, (LPVOID*)&OnReadyToStartMatchOG);

	Log("Hooked AFortPlayerControllerZone");
}