#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameMode.h"

#include "FortniteGame/Public/AI/FortAIController.h"
#include "FortniteGame/Public/AI/FortAIPawn.h"
#include "FortniteGame/Public/FortPlayerStart/FortPlayerStartWarmup.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortCharacter/FortCharacter.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortInventory/FortQuickBars.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

bool AFortGameMode::SpawnPlayerBot(AActor* SpawnPoint)
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortGameMode::SpawnPlayerBot: World is null!");
		return false;
	}

	static TArray<AActor*> PlayerStarts;
	if (PlayerStarts.Num() == 0) {
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerStartWarmup::StaticClass(), &PlayerStarts);
	}
	
	int32 MaxSpawnLocations = PlayerStarts.Num();
	static int32 BotSpawnLocationIndex = 0;

	if (Version::Fortnite_Version <= 1.72) {
		if (!SpawnPoint && PlayerStarts.Num() > 0) {
			if (BotSpawnLocationIndex >= MaxSpawnLocations) {
				BotSpawnLocationIndex = 0;
			}
			SpawnPoint = PlayerStarts.IsValidIndex(BotSpawnLocationIndex) ? PlayerStarts[BotSpawnLocationIndex] : nullptr;
			BotSpawnLocationIndex++;
		}

		if (!SpawnPoint) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to find spawn point!");
			return false;
		}

		FVector SpawnLocation = SpawnPoint->K2_GetActorLocation();
		FRotator SpawnRotation = SpawnPoint->K2_GetActorRotation();

		/*APlayerController* BotController = SpawnPlayerController(
			ROLE_SimulatedProxy,
			SpawnLocation,
			SpawnRotation
		);*/
		APlayerController* BotController = (APlayerController*)World->SpawnActor(StaticLoadObject<UClass>("/Game/Athena/Athena_PlayerController.Athena_PlayerController_C"), SpawnLocation, SpawnRotation);
		if (!BotController) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to spawn BotController!");
			return false;
		}
		HandleStartingNewPlayer(BotController);

		//BotController->InitPlayerState();
		BotController->PlayerState = (APlayerState*)World->SpawnActor(AFortPlayerStateAthena::StaticClass(), SpawnLocation, SpawnRotation, BotController);

		if (!BotController->PlayerState) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to initialize BotController's PlayerState!");
			return false;
		}
		BotController->PlayerState->bIsABot = true;
		BotController->OnRep_PlayerState();

		APawn* BotPawn = (APawn*)World->SpawnActor(
			AFortPlayerPawnAthena::StaticClass(),
			SpawnLocation,
			SpawnRotation
		);
		if (!BotPawn) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to manually spawn bot pawn!");
			return false;
		}

		BotController->Possess(BotPawn);
		if (BotController->Pawn != BotPawn) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to possess bot pawn!");
			return false;
		}
		BotController->OnRep_Pawn();

		if (!BotController->Pawn->PlayerState) {
			BotController->Pawn->PlayerState = BotController->PlayerState;
		}
		BotController->Pawn->OnRep_PlayerState();

		AFortPlayerStateAthena* FortPSAthena = BotController->PlayerState->Cast<AFortPlayerStateAthena>();
		AFortPlayerControllerAthena* FortPCAthena = BotController->Cast<AFortPlayerControllerAthena>();
		AFortPlayerPawnAthena* FortPlayerPawnAthena = BotController->Pawn->Cast<AFortPlayerPawnAthena>();
		if (FortPCAthena && FortPlayerPawnAthena) {
			FortPCAthena->Pawn = FortPlayerPawnAthena;
			FortPCAthena->MyFortPawn = FortPlayerPawnAthena;
			FortPCAthena->OnRep_Pawn();
			
			static UFortHeroType* Commando = (UFortHeroType*)StaticLoadObject("/Game/Athena/Heroes/HID_001_Athena_Commando_F.HID_001_Athena_Commando_F");
			static UFortHeroType* Commando2 = (UFortHeroType*)StaticLoadObject("/Game/Athena/Heroes/HID_Commando_Athena_01.HID_Commando_Athena_01");
			
			FortPSAthena->HeroType = Commando ? Commando : Commando2;
			FortPSAthena->OnRep_HeroType();

			if (Version::Fortnite_Version <= 1.72) {
				UCustomCharacterPart* HeadPart = (UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
				UCustomCharacterPart* BodyPart = (UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

				FortPlayerPawnAthena->ServerChoosePart(HeadPart, HeadPart->CharacterPartType);
				FortPlayerPawnAthena->ServerChoosePart(BodyPart, BodyPart->CharacterPartType);

				FortPSAthena->OnRep_CharacterParts();
			}
			else {
				// ApplyCharacterCustomization
			}

			UFortAbilitySet* FortAbilitySet = nullptr;

			if (Version::Fortnite_Version >= 2) {
				FortAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");
			}
			else {
				FortAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");
			}

			TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface = TScriptInterface<IAbilitySystemInterface>();
			AbilitySystemInterface.ObjectPointer = FortPSAthena;
			AbilitySystemInterface.InterfacePointer = FortPSAthena->GetInterfaceAddress(IAbilitySystemInterface::StaticClass());

			UFortKismetLibrary::EquipFortAbilitySet(AbilitySystemInterface, FortAbilitySet, FortPSAthena);

			if (!FortPCAthena->WorldInventory) {
				AFortInventory* NewInventory = (AFortInventory*)World->SpawnActor(AFortInventory::StaticClass());
				if (NewInventory) {
					FortPCAthena->WorldInventory = NewInventory;
				}
			}

			if (FortPCAthena->WorldInventory) {
				FortPCAthena->OnReadyToStartMatch(FortPCAthena);
				FortPCAthena->WorldInventory->EquipHarvestingTool();
			}
		}

		return true;
	}

	return false;
}