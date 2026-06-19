#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"
#include "FortniteGame/Public/FortPlayer/FortPlayerDeathReport.h"
#include "FortniteGame/Public/Info/FortTeamInfo.h"

void AFortPlayerControllerAthena::EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft) {
	EnterAircraftOG(This, InAircraft);
}

void AFortPlayerControllerAthena::ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation) {
	ServerAttemptAircraftJumpOG(This, ClientRotation);

	if (This->MyFortPawn) {
		This->MyFortPawn->ForceNetUpdate();
	}
}

void AFortPlayerControllerAthena::ClientOnPawnDied_Implementation(AFortPlayerControllerAthena* This, FFortPlayerDeathReport& DeathReport) {
	//ClientOnPawnDied_ImplementationOG(This, DeathReport);
	
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ClientOnPawnDied: World is null!");
		return;
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();
	if (!FortGameModeAthena || !FortGameStateAthena) {
		Log("ClientOnPawnDied: GameMode or GameState is null or not a FortGameModeAthena/FortGameStateAthena!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = This->PlayerState->Cast<AFortPlayerStateAthena>();
	if (!PlayerStateAthena) {
		Log("ClientOnPawnDied: PlayerState is null or not a FortPlayerStateAthena!");
		return;
	}

	FDeathInfo& DeathInfo = PlayerStateAthena->DeathInfo;

	AFortPlayerPawnAthena* PlayerPawnAthena = This->MyFortPawn->Cast<AFortPlayerPawnAthena>();
	if (!PlayerPawnAthena) {
		Log("ClientOnPawnDied: MyFortPawn is null or not a FortPlayerPawnAthena!");
		return;
	}

	AFortPlayerStateAthena* KillerPlayerStateAthena = DeathReport.KillerPlayerState->Cast<AFortPlayerStateAthena>();
	AFortPlayerPawnAthena* KillerPlayerPawnAthena = DeathReport.KillerPawn->Cast<AFortPlayerPawnAthena>();

	if (This->WorldInventory) {
		This->WorldInventory->DropAllItems();
	}

	bool bIsDBNO = false;
	if (KillerPlayerStateAthena->PlayerTeam) {
		for (AController* TeamMember : PlayerStateAthena->PlayerTeam->TeamMembers) {
			AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
			if (!TeamMemberController || TeamMemberController == This || !TeamMemberController->bMarkedAlive) {
				continue;
			}

			AFortPlayerPawnAthena* TeamMemberPlayerPawn = TeamMemberController->MyFortPawn->Cast<AFortPlayerPawnAthena>();
			if (!TeamMemberPlayerPawn || TeamMemberPlayerPawn->bIsDBNO) {
				continue;
			}

			bIsDBNO = true;
			break;
		}
	}

	DeathInfo.bDBNO = bIsDBNO;
	DeathInfo.FinisherOrDowner = KillerPlayerStateAthena ? KillerPlayerStateAthena : PlayerStateAthena;
	DeathInfo.DeathCause = AFortPlayerStateAthena::ToDeathCause(DeathReport.Tags, bIsDBNO);
	Log("==================== DeathInfo Dump Start ====================");
	Log("==================== bDBNO=" + std::to_string(DeathInfo.bDBNO));
	Log("==================== FinisherOrDowner=" + (DeathInfo.FinisherOrDowner ? DeathInfo.FinisherOrDowner->GetName().ToString() : "None"));
	Log("==================== DeathCause=" + std::to_string(DeathInfo.DeathCause));
	Log("==================== DeathInfo Dump End ====================");
	PlayerStateAthena->OnRep_DeathInfo();

	if (Version::Fortnite_Version >= 1.8) {
		if (KillerPlayerStateAthena && KillerPlayerStateAthena != PlayerStateAthena) {
			if (bIsDBNO) {
				KillerPlayerStateAthena->DownScore++;
				KillerPlayerStateAthena->ClientReportDBNO(PlayerStateAthena);
				KillerPlayerStateAthena->OnRep_Downs();
			}
			else {
				KillerPlayerStateAthena->KillScore++;
				if (Version::Fortnite_Version > 1.8) {
					KillerPlayerStateAthena->ClientReportKill(PlayerStateAthena);
				}
				KillerPlayerStateAthena->OnRep_Kills();

				if (KillerPlayerStateAthena->PlayerTeam) {
					for (AController* TeamMember : KillerPlayerStateAthena->PlayerTeam->TeamMembers) {
						AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
						if (TeamMemberController) {
							AFortPlayerStateAthena* TeamMemberPlayerState = TeamMemberController->PlayerState->Cast<AFortPlayerStateAthena>();
							if (TeamMemberPlayerState) {
								TeamMemberPlayerState->TeamKillScore++;
								TeamMemberPlayerState->ClientReportTeamKill(TeamMemberPlayerState->TeamKillScore);
								TeamMemberPlayerState->OnRep_TeamKillScore();
							}
						}
					}
				}
			}
		}

		if (Version::Fortnite_Version < 2.1) {
			if (FortGameModeAthena->bAllowSpectateAfterDeath) {
				APawn* PawnToSpectate = DeathReport.KillerPawn;
				if (!PawnToSpectate) {
					// Find next spectatable player
					for (AFortPlayerControllerAthena* PC : FortGameModeAthena->AlivePlayers) {
						if (PC && PC->Pawn) {
							PawnToSpectate = PC->Pawn;
							break;
						}
					}
				}

				if (PawnToSpectate) {
					This->PlayerToSpectateOnDeath = PawnToSpectate;

					FTimerHandle TimerHandle = UKismetSystemLibrary::K2_SetTimer(This, "SpectateOnDeath", 5.f, false);
					if (!TimerHandle.IsValid()) {
						Log("AFortPlayerControllerAthena::ClientOnPawnDied: Failed to set timer for SpectateOnDeath!");
					}
				}
				else {
					Log("AFortPlayerControllerAthena::ClientOnPawnDied: Unable to find a pawn to spectate after death!");
				}
			}
		}
	}

	TArray<FString> Medals;
	TArray<FFortQuestObjectiveCompletion> Advance;

	UFortQuestManager* QuestManager = This->GetQuestManager(ESubGame::GetAthena());
	if (QuestManager) {
		Advance = QuestManager->PendingChanges;
	}

	int32 MinutesAlive = PlayerStateAthena ? (PlayerStateAthena->SecondsAlive / 60) : -1;
	int32 PersonalKills = PlayerStateAthena ? PlayerStateAthena->KillScore : -1;
	int32 TeamKills = PlayerStateAthena ? PlayerStateAthena->TeamKillScore : -1;
	int32 Placement = PlayerStateAthena ? PlayerStateAthena->Place : -1;

	if (This->AthenaProfile) {
		FDedicatedServerUrlContext Context;
		This->AthenaProfile->EndBattleRoyaleGame(
			Advance,
			FortGameModeAthena->CurrentPlaylistId,
			MinutesAlive,
			PersonalKills,
			TeamKills,
			Placement,
			Medals,
			&Context
		);
	}

	Medals.Free();
	Advance.Free();
}

void AFortPlayerControllerAthena::OnReadyToStartMatch(AFortPlayerControllerAthena* This) {
	OnReadyToStartMatchOG(This);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerControllerAthena::OnReadyToStartMatch: World is null!");
		return;
	}

	if (This->CustomizationLoadout.Pickaxe) {
		This->WorldInventory->AddItem(This->CustomizationLoadout.Pickaxe->WeaponDefinition);
	}
}

void AFortPlayerControllerAthena::ServerReturnToMainMenu(AFortPlayerControllerAthena* This) {
	if (Version::Fortnite_Version <= 1.72 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		return ServerReturnToMainMenuOG(This);
	}

	AFortPlayerControllerZone::ServerReturnToMainMenuOG(This);
}