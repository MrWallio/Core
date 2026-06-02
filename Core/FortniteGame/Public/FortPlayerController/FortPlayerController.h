#pragma once
#include "pch.h"
#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"

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
struct FBuildingClassData;
struct FFortItemEntryStateValue;

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
	void OnReadyToStartMatch();
	static void OnReadyToStartMatchHK(AFortPlayerController* This);

	void SpawnQuickBars();

	void SetupQuickBars();

	static inline void (*ServerCheatOG)(AFortPlayerController* This, FString* Msg);
	void ServerCheat(FString* Msg);
	static void ServerCheatHK(AFortPlayerController* This, FString* Msg);

	static inline void (*ServerExecuteInventoryItemOG)(AFortPlayerController* This, FGuid& ItemGuid);
	void ServerExecuteInventoryItem(FGuid& ItemGuid);
	static void ServerExecuteInventoryItemHK(AFortPlayerController* This, FGuid& ItemGuid);

	FFortItemEntry* FindItemEntry(FGuid Guid);
	FFortItemEntry* FindItemEntry(UFortItemDefinition* ItemDefinition);

	UFortWorldItem* FindItemInstance(FGuid Guid);
	UFortWorldItem* FindItemInstance(UFortItemDefinition* ItemDefinition);

	void ClientReportDamagedResourceBuilding(ABuildingSMActor* BuildingSMActor, uint8 PotentialResourceType, int32 PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot);

	void ServerAttemptInventoryDrop(FGuid& ItemGuid, int Count, bool bTrash);
	static inline void (*ServerAttemptInventoryDropOG)(AFortPlayerController* This, FGuid& ItemGuid, int Count, bool bTrash);
	static void ServerAttemptInventoryDropHK(AFortPlayerController* This, FGuid& ItemGuid, int Count, bool bTrash);

	void ClientForceUpdateQuickbar(uint8 QuickbarToRefresh);

	void OnRep_QuickBar();

	static bool IsUsingOldQuickBars();

	static inline void (*ServerClientPawnLoadedOG)(AFortPlayerController* This, bool bIsPawnLoaded);
	void ServerClientPawnLoaded(bool bIsPawnLoaded);
	static void ServerClientPawnLoadedHK(AFortPlayerController* This, bool bIsPawnLoaded);

	static inline bool (*RemoveInventoryItemOG)(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoval);
	bool RemoveInventoryItem(FGuid& ItemGuid, int32 Count, bool bForceRemoval);
	static bool RemoveInventoryItemHK(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoval);

	static inline void (*ServerCreateBuildingActorOldOG)(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored);
	void ServerCreateBuildingActorOld(FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored);
	static void ServerCreateBuildingActorOldHK(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored);

	bool CanAffordToPlaceBuildableClass(FBuildingClassData* ClassToBuildData);

	int32 PayBuildableClassPlacementCost(FBuildingClassData* ClassToBuildData);

	static inline void (*ServerBeginEditingBuildingActorOG)(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit);
	void ServerBeginEditingBuildingActor(ABuildingSMActor* BuildingActorToEdit);
	static void ServerBeginEditingBuildingActorHK(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit);

	static inline void (*ServerEditBuildingActorOG)(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored);
	void ServerEditBuildingActor(ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored);
	static void ServerEditBuildingActorHK(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored);

	static inline void (*ServerEndEditingBuildingActorOG)(AFortPlayerController* This, ABuildingSMActor* BuildingActorToStopEditing);
	void ServerEndEditingBuildingActor(ABuildingSMActor* BuildingActorToStopEditing);
	static void ServerEndEditingBuildingActorHK(AFortPlayerController* This, ABuildingSMActor* BuildingActorToStopEditing);

	static inline void (*ServerAttemptInteractOG)(AFortPlayerController* This, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType);
	void ServerAttemptInteract(AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType);
	static void ServerAttemptInteractHK(AFortPlayerController* This, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType);

	void ServerRemoveInventoryStateValue(FGuid& ItemGuid, uint8 StateValueType);
	static inline void (*ServerRemoveInventoryStateValueOG)(AFortPlayerController* This, FGuid& ItemGuid, uint8 StateValueType);
	static void ServerRemoveInventoryStateValueHK(AFortPlayerController* This, FGuid& ItemGuid, uint8 StateValueType);

	void ServerSetInventoryStateValue(FGuid& ItemGuid, FFortItemEntryStateValue& StateValue);
	static inline void (*ServerSetInventoryStateValueOG)(AFortPlayerController* This, FGuid& ItemGuid, FFortItemEntryStateValue& StateValue);
	static void ServerSetInventoryStateValueHK(AFortPlayerController* This, FGuid& ItemGuid, FFortItemEntryStateValue& StateValue);

	static void Hook() {
		/*HookVTableIdx(
			AFortPlayerController::GetDefaultObj(),
			Finder::FindAFortPlayerController_OnReadyToStartMatchVFT(),
			OnReadyToStartMatch,
			(LPVOID*)&OnReadyToStartMatchOG
		);*/
		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(
				AFortPlayerController::GetDefaultObj(),
				Finder::FindAFortPlayerController_OnReadyToStartMatchVFT()
			)),
			OnReadyToStartMatchHK,
			(LPVOID*)&OnReadyToStartMatchOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerCheat"),
			ServerCheatHK,
			(LPVOID*)&ServerCheatOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerExecuteInventoryItem"),
			ServerExecuteInventoryItemHK,
			(LPVOID*)&ServerExecuteInventoryItemOG
		);

		UFunction* ServerAttemptInventoryDropFunc = AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInventoryDrop");
		if (ServerAttemptInventoryDropFunc) {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				ServerAttemptInventoryDropFunc,
				ServerAttemptInventoryDropHK,
				(LPVOID*)&ServerAttemptInventoryDropOG
			);
		}
		else {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerSpawnInventoryDrop"),
				ServerAttemptInventoryDropHK,
				(LPVOID*)&ServerAttemptInventoryDropOG
			);
		}

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerClientPawnLoaded"),
			ServerClientPawnLoadedHK,
			(LPVOID*)&ServerClientPawnLoadedOG
		);

		/*HookEveryVTableIdx(
			AFortPlayerController::StaticClass(),
			Finder::FindAFortPlayerController_RemoveInventoryItemVFT(),
			RemoveInventoryItem,
			(LPVOID*)&RemoveInventoryItemOG
		);*/
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerController_RemoveInventoryItem()), RemoveInventoryItemHK, (LPVOID*)&RemoveInventoryItemOG);

		if (Version::Fortnite_Version <= 1.82) {
			HookEveryVTable(
				AFortPlayerController::StaticClass(),
				AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerCreateBuildingActor"),
				ServerCreateBuildingActorOldHK,
				(LPVOID*)&ServerCreateBuildingActorOldOG
			);
		}

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerBeginEditingBuildingActor"),
			ServerBeginEditingBuildingActorHK,
			(LPVOID*)&ServerBeginEditingBuildingActorOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerEditBuildingActor"),
			ServerEditBuildingActorHK,
			(LPVOID*)&ServerEditBuildingActorOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerEndEditingBuildingActor"),
			ServerEndEditingBuildingActorHK,
			(LPVOID*)&ServerEndEditingBuildingActorOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInteract"),
			ServerAttemptInteractHK,
			(LPVOID*)&ServerAttemptInteractOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerRemoveInventoryStateValue"),
			ServerRemoveInventoryStateValueHK,
			(LPVOID*)&ServerRemoveInventoryStateValueOG
		);

		HookEveryVTable(
			AFortPlayerController::StaticClass(),
			AFortPlayerController::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerSetInventoryStateValue"),
			ServerSetInventoryStateValueHK,
			(LPVOID*)&ServerSetInventoryStateValueOG
		);

		Log("Hooked AFortPlayerController");
	}
};