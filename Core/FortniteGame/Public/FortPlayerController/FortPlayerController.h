#pragma once
#include "pch.h"
#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

class AFortPlayerPawn;
class ABuildingSMActor;
struct FCreateBuildingActorData;
enum class EFortResourceType : uint8;
class UFortMontageItemDefinitionBase;
class AFortQuickBars;
class AFortInventory;
class FFortItemEntry;
class UFortWorldItem;
class UFortItemDefinition;
class UFortRegisteredPlayerInfo;
class UFortMcpProfileWorld;

class AFortPlayerController : public APlayerController {
public:
	DefineUnrealClass(AFortPlayerController);

	DefineUProperty(AFortPlayerPawn*, MyFortPawn);

	DefineUProperty(AFortInventory*, WorldInventory);

	DefineBitfieldUProperty(bBuildFree);

	DefineUProperty(AFortQuickBars*, ClientQuickBars);

	DefineUProperty(AFortQuickBars*, QuickBars);

	DefineUProperty(bool, bHasClientFinishedLoading);

	DefineUProperty(bool, bHasServerFinishedLoading);

	DefineUProperty(UFortRegisteredPlayerInfo*, MyPlayerInfo);
public:
	void ClientForceProfileQuery();

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerController* This);
	static void OnReadyToStartMatch(AFortPlayerController* This);

	void SpawnQuickBars();

	void SetupQuickBars();

	static inline void (*ServerCheatOG)(AFortPlayerController* This, FString* Msg);
	static void ServerCheat(AFortPlayerController* This, FString* Msg);

	static inline void (*ServerExecuteInventoryItemOG)(AFortPlayerController* This, FGuid& ItemGuid);
	static void ServerExecuteInventoryItem(AFortPlayerController* This, FGuid& ItemGuid);

	FFortItemEntry* FindItemEntry(FGuid Guid);
	FFortItemEntry* FindItemEntry(UFortItemDefinition* ItemDefinition);

	UFortWorldItem* FindItemInstance(FGuid Guid);
	UFortWorldItem* FindItemInstance(UFortItemDefinition* ItemDefinition);

	void ClientReportDamagedResourceBuilding(ABuildingSMActor* BuildingSMActor, uint8 PotentialResourceType, int32 PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot);

	static void ServerAttemptInventoryDrop(AFortPlayerController* This, FGuid& ItemGuid, int Count, bool bTrash);

	void ClientForceUpdateQuickbar(uint8 QuickbarToRefresh);

	void OnRep_QuickBar();

	static bool IsUsingOldQuickBars();

	static void Hook() {
		/*HookVTableIdx(
			AFortPlayerController::GetDefaultObj(),
			Finder::FindAFortPlayerController_OnReadyToStartMatchVFT(),
			OnReadyToStartMatch,
			(LPVOID*)&OnReadyToStartMatchOG
		);*/
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerController_OnReadyToStartMatch()), OnReadyToStartMatch, (LPVOID*)&OnReadyToStartMatchOG);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerCheat"),
			ServerCheat,
			(LPVOID*)&ServerCheatOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerExecuteInventoryItem"),
			ServerExecuteInventoryItem,
			(LPVOID*)&ServerExecuteInventoryItemOG
		);

		UFunction* ServerAttemptInventoryDropFunc = AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInventoryDrop");
		if (ServerAttemptInventoryDropFunc) {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				ServerAttemptInventoryDropFunc,
				ServerAttemptInventoryDrop,
				nullptr
			);
		}
		else {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerSpawnInventoryDrop"),
				ServerAttemptInventoryDrop,
				nullptr
			);
		}

		Log("Hooked AFortPlayerController");
	}
};