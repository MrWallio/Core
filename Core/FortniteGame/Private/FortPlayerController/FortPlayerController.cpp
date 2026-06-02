#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerController.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/Cheat/FortCheatManager.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/Info/FortRegisteredPlayerInfo.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/BuildingActor/BuildingSMActor.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"

void AFortPlayerController::ClientForceProfileQuery()
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName("ClientForceProfileQuery"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ClientForceProfileQueryInternal)(AFortPlayerController*) = decltype(ClientForceProfileQueryInternal)(VTable[VTableIdx]);
		return ClientForceProfileQueryInternal(this);
	}
}

void AFortPlayerController::OnReadyToStartMatch() {
	OnReadyToStartMatchOG(this);

	if ((!QuickBars || !ClientQuickBars))
	{
		SpawnQuickBars();
		SetupQuickBars();
	}
}

void AFortPlayerController::OnReadyToStartMatchHK(AFortPlayerController* This) {
	if (!This) return OnReadyToStartMatchOG(This);

	This->OnReadyToStartMatch();
}

void AFortPlayerController::SpawnQuickBars()
{
	if (Finder::FindAFortPlayerController_SpawnQuickBarsVFT() != 0) {
		void (*&SpawnQuickBarsInternal)(AFortPlayerController* This) = decltype(SpawnQuickBarsInternal)(VTable[Finder::FindAFortPlayerController_SpawnQuickBarsVFT()]);
		SpawnQuickBarsInternal(this);
	}
	else {
		UWorld* World = UWorld::GetWorld();
		if (!World) {
			Log("AFortPlayerController::SpawnQuickBars: World is null!");
			return;
		}

		if (IsUsingOldQuickBars()) {
			if (!QuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBars::StaticClass(), FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					QuickBars = NewQuickBars->Cast<AFortQuickBars>();
					Log("Spawned QuickBars: " + QuickBars->GetName().ToString());
				}
			}
		}
		else {
			if (!ClientQuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBars::StaticClass(), FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					ClientQuickBars = NewQuickBars->Cast<AFortQuickBars>();
					Log("Spawned ClientQuickBars: " + ClientQuickBars->GetName().ToString());
				}
			}
		}
	}
}

void AFortPlayerController::SetupQuickBars()
{
	if (Finder::FindAFortPlayerController_SetupQuickBarsVFT()) {
		void (*&SetupQuickBarsInternal)(AFortPlayerController* This) = decltype(SetupQuickBarsInternal)(VTable[Finder::FindAFortPlayerController_SetupQuickBarsVFT()]);
		SetupQuickBarsInternal(this);
	}
}

void AFortPlayerController::ServerCheat(FString* Msg) {
	std::string Command = Msg->ToString();
	Log("ServerCheat (" + GetName().ToString() + "): [" + Command + "]");
	
	UFortCheatManager* CheatManager = CheatManager->Cast<UFortCheatManager>();
	if (!CheatManager)
	{
		UFortCheatManager* NewCheatManager = (UFortCheatManager*)UGameplayStatics::SpawnObject(CheatClass.Get(), this);
		if (NewCheatManager) {
			CheatManager = NewCheatManager;
			CheatManager = NewCheatManager;
			ClientMessage("Spawned CheatManager: " + CheatManager->GetName().ToString());
		}
		else {
			ClientMessage("Failed to spawn CheatManager!");
			return;
		}
	}

	ServerCheatOG(this, Msg);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		ClientMessage("World is null!");
		return;
	}

	AFortGameMode* GameMode = World->AuthorityGameMode->Cast<AFortGameMode>();
	if (!GameMode) {
		ClientMessage("GameMode is null or not a FortGameMode!");
		return;
	}

	FCommandParser Parser(Command);

	if (Parser.IsCommand("Help"))
	{
		ClientMessage("=== Available Commands ===");
		ClientMessage("GiveItem <ItemDefinitionName> [Count] - Gives an item to the player's inventory.");
		ClientMessage("SetLoadedAmmo <LoadedAmmo> - Sets the loaded ammo of the currently equipped weapon.");
		ClientMessage("GiveAmmo [Amount] - Gives ammo for the currently equipped weapon.");
		ClientMessage("DumpInventory - Dump Inventory Stats");
		ClientMessage("SpawnBot [bSpawnAtPlayer] - Spawns a bot at the player's location or playerstart.");
		ClientMessage("SetHealth <Health> - Sets the player's health.");
		ClientMessage("SetShield <Shield> - Sets the player's shield.");
		ClientMessage("SetMaxHealth <MaxHealth> - Sets the player's max health.");
		ClientMessage("SetMaxShield <MaxShield> - Sets the player's max shield.");
	}
	else if (Parser.IsCommand("GiveItem")) {
		if (Parser.GetArgCount() < 1)
		{
			ClientMessage("Usage: GiveItem <ItemDefinitionName> [Count]");
			return;
		}

		if (!WorldInventory) {
			ClientMessage("WorldInventory is null!");
			return;
		}
		
		std::string ItemDefName = Parser.GetArg(0);
		int32 Count = Parser.GetArgInt(1, 1);

		UObject* ItemObj;
		if (ItemDefName.contains("/")) {
			if (ItemDefName.starts_with("FortniteGame/"))
			{
				ItemDefName = "/Game/" + ItemDefName.substr(strlen("FortniteGame/"));
			}

			size_t contentPos = ItemDefName.find("/Content/");
			if (contentPos != std::string::npos)
			{
				if (ItemDefName.contains("/Game/Content/"))
				{
					ItemDefName.replace(ItemDefName.find("/Game/Content/"), strlen("/Game/Content/"), "/Game/");
				}
				else
				{
					size_t contentPos = ItemDefName.find("/Content/");
					ItemDefName = ItemDefName.substr(0, contentPos)
						+ "/Game/"
						+ ItemDefName.substr(contentPos + strlen("/Content/"));
				}
			}

			if (!ItemDefName.contains("."))
			{
				size_t lastSlash = ItemDefName.find_last_of('/');
				if (lastSlash != std::string::npos)
				{
					std::string className = ItemDefName.substr(lastSlash + 1);
					ItemDefName += "." + className;
				}
				else
				{
					ClientMessage("Invalid ItemDefinition path: " + ItemDefName);
					return;
				}
			}

			ItemObj = StaticLoadObject(ItemDefName);
		}
		else {
			ItemObj = FUObjectArray::FindObjectFast(ItemDefName);
		}
		if (!ItemObj) {
			ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		WorldInventory->AddItemAndHandleOverflow(ItemDef, Count);
	}
	else if (Parser.IsCommand("SetLoadedAmmo")) {
		if (Parser.GetArgCount() < 1)
		{
			ClientMessage("Usage: SetLoadedAmmo <LoadedAmmo>");
			return;
		}

		int32 LoadedAmmo = Parser.GetArgInt(0, 30);

		if (!MyFortPawn) {
			ClientMessage("MyFortPawn is null!");
			return;
		}
		AFortWeapon* CurrentWeapon = MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			ClientMessage("CurrentWeapon is null!");
			return;
		}

		FFortItemEntry* WeaponEntry = FindItemEntry(CurrentWeapon->ItemEntryGuid);
		if (!WeaponEntry) {
			ClientMessage("Weapon entry not found in inventory!");
			return;
		}

		WeaponEntry->LoadedAmmo = LoadedAmmo;
		WorldInventory->Update(WeaponEntry);
		ClientMessage("Set loaded ammo of current weapon to " + std::to_string(LoadedAmmo));
	}
	else if (Parser.IsCommand("GiveAmmo")) {
		int32 AmmoAmount = Parser.GetArgInt(0, 30);

		if (!MyFortPawn) {
			ClientMessage("MyFortPawn is null!");
			return;
		}

		AFortWeapon* CurrentWeapon = MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			ClientMessage("CurrentWeapon is null!");
			return;
		}

		if (!CurrentWeapon->WeaponData) {
			ClientMessage("WeaponData is null!");
			return;
		}

		UFortWorldItemDefinition* ItemDef = CurrentWeapon->WeaponData->Cast<UFortWorldItemDefinition>();
		if (!ItemDef) {
			ClientMessage("WeaponData is not a UFortWorldItemDefinition!");
			return;
		}

		UFortWorldItemDefinition* AmmoItemDef = ItemDef->GetAmmoWorldItemDefinition_BP();
		if (!AmmoItemDef) {
			ClientMessage("AmmoWorldItemDefinition is null!");
			return;
		}

		FFortItemEntry* AmmoEntry = FindItemEntry(AmmoItemDef);
		if (AmmoEntry) {
			AmmoEntry->Count += AmmoAmount;
			WorldInventory->Update(AmmoEntry);
			ClientMessage("Added " + AmmoItemDef->GetName().ToString() + " " + std::to_string(AmmoAmount) + " ammo to existing stack.");
		}
		else {
			AmmoEntry = WorldInventory->AddItem(AmmoItemDef, AmmoAmount);
			if (AmmoEntry) {
				ClientMessage("Added new ammo entry: " + AmmoItemDef->GetName().ToString() + " x" + std::to_string(AmmoAmount));
			}
			else {
				ClientMessage("Failed to add ammo item: " + AmmoItemDef->GetName().ToString());
			}
		}
	} else if (Parser.IsCommand("DumpInventory")) {
		if (!WorldInventory) {
			ClientMessage("WorldInventory is null!");
			return;
		}
		ClientMessage("=== Inventory Dump ===");

		ClientMessage("Stats: ");
		ClientMessage("Inventory Capacity: " + std::to_string(WorldInventory->GetInventoryCapacity()));
		ClientMessage("Inventory Used: " + std::to_string(WorldInventory->GetInventoryUsed()));

		ClientMessage("Items: ");
		for (UFortWorldItem* ItemEntry : WorldInventory->Inventory.ItemInstances) {
			std::string ItemName = ItemEntry->ItemEntry.ItemDefinition ? ItemEntry->ItemEntry.ItemDefinition->GetName().ToString() : "Unknown";
			int32 Count = ItemEntry->ItemEntry.Count;
			std::string FormattedGuid = ItemEntry->ItemEntry.ItemGuid.FormatGuid();

			ClientMessage("Item: " + ItemName + ", Count: " + std::to_string(Count) + ", GUID: " + FormattedGuid);
		}

		ClientMessage("=== End of Inventory Dump ===");
	}
	else if (Parser.IsCommand("SpawnBot")) {
		bool bSpawnAtPlayer = Parser.GetArgBool(0, false);
		
		bool bSuccessfulSpawn = GameMode->SpawnPlayerBot(bSpawnAtPlayer ? MyFortPawn : nullptr);
		if (bSuccessfulSpawn) {
			ClientMessage("Bot spawned successfully!");
		}
		else {
			ClientMessage("Failed to spawn bot.");
		}
	} else if (Parser.IsCommand("SetHealth")) {
		if (Parser.GetArgCount() < 1)
		{
			ClientMessage("Usage: SetHealth <Health>");
			return;
		}

		float Health = Parser.GetArgFloat(0, 100.0f);

		if (!MyFortPawn) {
			ClientMessage("MyFortPawn is null!");
			return;
		}

		MyFortPawn->SetHealth(Health);

		ClientMessage("Set health to " + std::to_string(Health));
	} else if (Parser.IsCommand("SetShield")) {
		if (Parser.GetArgCount() < 1)
		{
			ClientMessage("Usage: SetShield <Shield>");
			return;
		}

		float Shield = Parser.GetArgFloat(0, 100.0f);

		if (!MyFortPawn) {
			ClientMessage("MyFortPawn is null!");
			return;
		}

		MyFortPawn->SetShield(Shield);

		ClientMessage("Set shield to " + std::to_string(Shield));
	} else if (Parser.IsCommand("SetMaxHealth")) {
		if (Parser.GetArgCount() < 1)
		{
			ClientMessage("Usage: SetMaxHealth <MaxHealth>");
			return;
		}

		float MaxHealth = Parser.GetArgFloat(0, 100.0f);

		if (!MyFortPawn) {
			ClientMessage("MyFortPawn is null!");
			return;
		}

		MyFortPawn->SetMaxHealth(MaxHealth);

		ClientMessage("Set max health to " + std::to_string(MaxHealth));
	} else if (Parser.IsCommand("SetMaxShield")) {
		if (Parser.GetArgCount() < 1)
		{
			ClientMessage("Usage: SetMaxShield <MaxShield>");
			return;
		}

		float MaxShield = Parser.GetArgFloat(0, 100.0f);

		if (!MyFortPawn) {
			ClientMessage("MyFortPawn is null!");
			return;
		}

		MyFortPawn->SetMaxShield(MaxShield);

		ClientMessage("Set max shield to " + std::to_string(MaxShield));
	}
}

void AFortPlayerController::ServerCheatHK(AFortPlayerController* This, FString* Msg) {
	if (!This) return ServerCheatOG(This, Msg);

	This->ServerCheat(Msg);
}

void AFortPlayerController::ServerExecuteInventoryItem(FGuid& ItemGuid) {
	UFortWorldItem* ItemInstance = FindItemInstance(ItemGuid);
	if (!ItemInstance) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: ItemInstance not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	UFortWorldItemDefinition* ItemDef = ItemInstance->ItemEntry.ItemDefinition->Cast<UFortWorldItemDefinition>();
	if (!ItemDef) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: ItemDefinition is not a UFortWorldItemDefinition for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	ItemDef->ServerExecute(ItemInstance, this);
}

void AFortPlayerController::ServerExecuteInventoryItemHK(AFortPlayerController* This, FGuid& ItemGuid) {
	if (!This) return ServerExecuteInventoryItemOG(This, ItemGuid);

	This->ServerExecuteInventoryItem(ItemGuid);
}

FFortItemEntry* AFortPlayerController::FindItemEntry(FGuid Guid) {
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemEntry: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemEntry(Guid);
}

FFortItemEntry* AFortPlayerController::FindItemEntry(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition) {
		Log("AFortPlayerController::FindItemEntry: ItemDefinition is null!");
		return nullptr;
	}
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemEntry: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemEntry(ItemDefinition);
}

UFortWorldItem* AFortPlayerController::FindItemInstance(FGuid Guid)
{
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemInstance: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemInstance(Guid);
}

UFortWorldItem* AFortPlayerController::FindItemInstance(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition) {
		Log("AFortPlayerController::FindItemInstance: ItemDefinition is null!");
		return nullptr;
	}
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemInstance: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemInstance(ItemDefinition);
}

void AFortPlayerController::ClientReportDamagedResourceBuilding(ABuildingSMActor* BuildingSMActor, uint8 PotentialResourceType, int32 PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportDamagedResourceBuilding");

	struct FortPlayerController_ClientReportDamagedResourceBuilding
	{
	public:
		ABuildingSMActor* BuildingSMActor;
		uint8 PotentialResourceType;
		int32 PotentialResourceCount;
		bool bDestroyed;
		bool bJustHitWeakspot;
	};

	FortPlayerController_ClientReportDamagedResourceBuilding Parms{};

	Parms.BuildingSMActor = BuildingSMActor;
	Parms.PotentialResourceType = PotentialResourceType;
	Parms.PotentialResourceCount = PotentialResourceCount;
	Parms.bDestroyed = bDestroyed;
	Parms.bJustHitWeakspot = bJustHitWeakspot;

	ProcessEvent(Func, &Parms);
}

void AFortPlayerController::ServerAttemptInventoryDrop(FGuid& ItemGuid, int Count, bool bTrash) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: World is null!");
		return;
	}

	if (!MyFortPawn) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: MyFortPawn is null!");
		return;
	}

	FVector PawnLocation = MyFortPawn->K2_GetActorLocation();

	Log("AFortPlayerController::ServerAttemptInventoryDrop: Attempting to drop item with GUID: " + ItemGuid.FormatGuid() + ", Count: " + std::to_string(Count) + ", bTrash: " + std::to_string(bTrash));
	FFortItemEntry* ItemEntry = FindItemEntry(ItemGuid);
	if (ItemEntry) {
		if (ItemEntry->ItemDefinition) {
			if (!bTrash) {
				WorldInventory->SpawnPickupFromEntry(*ItemEntry);
			}
			WorldInventory->RemoveItem(ItemEntry->ItemGuid, Count);
		}
	}
}

void AFortPlayerController::ServerAttemptInventoryDropHK(AFortPlayerController* This, FGuid& ItemGuid, int Count, bool bTrash) {
	if (!This) return ServerAttemptInventoryDropOG(This, ItemGuid, Count, bTrash);

	This->ServerAttemptInventoryDrop(ItemGuid, Count, bTrash);
}

void AFortPlayerController::ClientForceUpdateQuickbar(uint8 QuickbarToRefresh)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientForceUpdateQuickbar");

	struct FortPlayerController_ClientForceUpdateQuickbar
	{
	public:
		uint8 QuickbarToRefresh;
	};

	FortPlayerController_ClientForceUpdateQuickbar Parms{};

	Parms.QuickbarToRefresh = QuickbarToRefresh;

	ProcessEvent(Func, &Parms);
}

void AFortPlayerController::OnRep_QuickBar()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_QuickBar");

	ProcessEvent(Func, nullptr);
}

bool AFortPlayerController::IsUsingOldQuickBars() {
	static uintptr_t ClientQuickBarsOffset = StaticClass()->GetPropertyOffset("ClientQuickBars");

	return ClientQuickBarsOffset == -1;
}

void AFortPlayerController::ServerClientPawnLoaded(bool bIsPawnLoaded)
{
	ServerClientPawnLoadedOG(this, bIsPawnLoaded);
	AFortPlayerState* FortPlayerState = PlayerState->Cast<AFortPlayerState>();
	if (!FortPlayerState) {
		Log("ServerAcknowledgePossession: PlayerState is null or not AFortPlayerStateZone");
		return;
	}

	AFortPlayerPawn* MyFortPawn = MyFortPawn;

	AFortPlayerControllerAthena* FortPCAthena = Cast<AFortPlayerControllerAthena>();

	if (bIsPawnLoaded) {
		if (Version::Fortnite_Version <= 1.91 && Version::Fortnite_Version != 1.1 && Version::Fortnite_Version != 1.11) {
			if (FortPCAthena && MyFortPawn) {
				if (FortPCAthena->StrongMyHero && FortPCAthena->StrongMyHero->CharacterParts.Num() > 0) {
					for (UCustomCharacterPart* CharacterPart : FortPCAthena->StrongMyHero->CharacterParts) {
						MyFortPawn->ServerChoosePart(CharacterPart, CharacterPart->CharacterPartType);
					}
				}
			}

			FortPlayerState->OnRep_CharacterParts();
			
			Log("ServerClientPawnLoaded: Applied CharacterParts for PlayerState " + FortPlayerState->GetName().ToString());
		}
		else {
			FortPlayerState->ApplyCharacterCustomization(MyFortPawn);
			Log("ServerClientPawnLoaded: Applied character customization for PlayerState " + FortPlayerState->GetName().ToString());
		}
	}
}

void AFortPlayerController::ServerClientPawnLoadedHK(AFortPlayerController* This, bool bIsPawnLoaded) {
	if (!This) return ServerClientPawnLoadedOG(This, bIsPawnLoaded);

	This->ServerClientPawnLoaded(bIsPawnLoaded);
}

bool AFortPlayerController::RemoveInventoryItem(FGuid& ItemGuid, int32 Count, bool bForceRemoval)
{
	static auto InterfaceOffset = StaticClass()->SuperStruct->PropertiesSize + (Version::Engine_Version >= 4.27 ? 16 : 8);
	AFortPlayerController* PlayerController = (AFortPlayerController*)(__int64(this) - InterfaceOffset); // this is so wierd
	if (!PlayerController) {
		Log("RemoveInventoryItem: Failed to get PlayerController from interface pointer!");
		return RemoveInventoryItemOG(this, ItemGuid, Count, bForceRemoval);
	}

	AFortInventory* Inventory = PlayerController->WorldInventory;
	if (!Inventory) {
		Log("RemoveInventoryItem: WorldInventory is null!");
		return RemoveInventoryItemOG(this, ItemGuid, Count, bForceRemoval);
	}

	if (!ItemGuid.IsValid()) {
		Log("RemoveInventoryItem: Invalid ItemGuid!");
		return RemoveInventoryItemOG(this, ItemGuid, Count, bForceRemoval);
	}

	return Inventory->RemoveItem(ItemGuid, Count);
}

bool AFortPlayerController::RemoveInventoryItemHK(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoval) {
	if (!This) return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoval);

	return This->RemoveInventoryItem(ItemGuid, Count, bForceRemoval);
}

void AFortPlayerController::ServerCreateBuildingActorOld(FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerCreateBuildingActor: World is null!");
		return;
	}

	UFortResourceItemDefinition* Resource = UFortKismetLibrary::K2_GetResourceItemDefinition(((ABuildingSMActor*)BuildingClassData.BuildingClass->GetDefaultObj())->ResourceType);
	if (!Resource) {
		Log("ServerCreateBuildingActor: Failed to get resource item definition for building class!");
		return;
	}

	if (!CanAffordToPlaceBuildableClass(&BuildingClassData)) {
		Log("ServerCreateBuildingActor: Cannot afford to place building!");
		return;
	}

	TArray<ABuildingActor*> BuildingsToRemove;
	uint8 OptionalAdjustment;
	if (UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(World, BuildingClassData.BuildingClass.Get(), BuildLoc, BuildRot, bMirrored, &BuildingsToRemove, &OptionalAdjustment)) {
		Log("ServerCreateBuildingActor: Cannot place building at location!");
		return;
	}

	for (ABuildingActor* BuildingToRemove : BuildingsToRemove) {
		if (BuildingToRemove) {
			BuildingToRemove->K2_DestroyActor();
		}
	}
	BuildingsToRemove.Free();

	ABuildingActor* BuildingActor = World->SpawnActorUnfinished(BuildingClassData.BuildingClass.Class, BuildLoc, BuildRot, this)->Cast<ABuildingActor>();
	if (!BuildingActor) {
		Log("ServerCreateBuildingActor: Failed to spawn building actor!");
		return;
	}
	BuildingActor->InitializeKismetSpawnedBuildingActor(BuildingActor, this, true, nullptr);
	World->FinishSpawnActor(BuildingActor, BuildLoc, BuildRot);

	BuildingActor->CurrentBuildingLevel = BuildingClassData.UpgradeLevel;
	BuildingActor->OnRep_CurrentBuildingLevel();

	ABuildingSMActor* BuildingSMActor = BuildingActor->Cast<ABuildingSMActor>();
	if (BuildingSMActor) {
		BuildingSMActor->bMirrored = bMirrored;
	}

	BuildingActor->bPlayerPlaced = true;

	AFortPlayerStateAthena* PlayerState = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerState) {
		BuildingActor->Team = PlayerState->TeamIndex;
	}
	BuildingActor->TeamIndex = BuildingActor->Team;

	PayBuildableClassPlacementCost(&BuildingClassData);
}

void AFortPlayerController::ServerCreateBuildingActorOldHK(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored) {
	if (!This) return ServerCreateBuildingActorOldOG(This, BuildingClassData, BuildLoc, BuildRot, bMirrored);

	This->ServerCreateBuildingActorOld(BuildingClassData, BuildLoc, BuildRot, bMirrored);
}

bool AFortPlayerController::CanAffordToPlaceBuildableClass(FBuildingClassData* ClassToBuildData) {
	bool (*&CanAffordToPlaceBuildableClassInternal)(AFortPlayerController * This, FBuildingClassData * ClassToBuildData) = decltype(CanAffordToPlaceBuildableClassInternal)(VTable[Finder::FindAFortPlayerController_CanAffordToPlaceBuildableClassVFT()]);
	return CanAffordToPlaceBuildableClassInternal(this, ClassToBuildData);
}

int32 AFortPlayerController::PayBuildableClassPlacementCost(FBuildingClassData* ClassToBuildData) {
	int32(*&PayBuildableClassPlacementCostInternal)(AFortPlayerController * This, FBuildingClassData * ClassToBuildData) = decltype(PayBuildableClassPlacementCostInternal)(VTable[Finder::FindAFortPlayerController_PayBuildableClassPlacementCostVFT()]);
	return PayBuildableClassPlacementCostInternal(this, ClassToBuildData);
}

void AFortPlayerController::ServerBeginEditingBuildingActor(ABuildingSMActor* BuildingActorToEdit) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerBeginEditingBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToEdit) {
		Log("ServerBeginEditingBuildingActor: BuildingActorToEdit is null!");
		return;
	}

	AFortPlayerStateZone* PlayerState = PlayerState->Cast<AFortPlayerStateZone>();
	if (!PlayerState) {
		Log("ServerBeginEditingBuildingActor: PlayerState is null or not AFortPlayerStateZone!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerStateAthena) {
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team) {
			Log("ServerBeginEditingBuildingActor: Player is not on the same team as the building, cannot edit. Player hacking?");
			return;
		}
	}

	BuildingActorToEdit->SetEditingPlayer(PlayerState);

	AFortPawn* MyFortPawn = MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerBeginEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	if (!MyFortPawn->CurrentWeapon->Cast<AFortWeap_EditingTool>()) {
		FFortItemEntry* ItemEntry = WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
		if (ItemEntry) {
			ServerExecuteInventoryItem(ItemEntry->ItemGuid);
		}
		else {
			Log("ServerBeginEditingBuildingActor: Failed to find edit tool in inventory!");
			return;
		}
	}

	AFortWeap_EditingTool* EditingTool = MyFortPawn->CurrentWeapon->Cast<AFortWeap_EditingTool>();
	if (!EditingTool) {
		Log("ServerBeginEditingBuildingActor: Current weapon is not the editing tool!");
		return;
	}

	EditingTool->EditActor = BuildingActorToEdit;
	EditingTool->ForceNetUpdate();
	EditingTool->OnRep_EditActor();
}

void AFortPlayerController::ServerBeginEditingBuildingActorHK(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit) {
	if (!This) return ServerBeginEditingBuildingActorOG(This, BuildingActorToEdit);

	This->ServerBeginEditingBuildingActor(BuildingActorToEdit);
}

void AFortPlayerController::ServerEditBuildingActor(ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerEditBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToEdit) {
		Log("ServerEditBuildingActor: BuildingActorToEdit is null!");
		return;
	}

	if (!NewBuildingClass) {
		Log("ServerEditBuildingActor: NewBuildingClass is null!");
		return;
	}

	AFortPawn* MyFortPawn = MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerEndEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	AFortPlayerStateZone* PlayerState = PlayerState->Cast<AFortPlayerStateZone>();
	if (!PlayerState) {
		Log("ServerEditBuildingActor: PlayerState is null or not AFortPlayerStateZone!");
		return;
	}

	if (BuildingActorToEdit->EditingPlayer != PlayerState) {
		Log("ServerEditBuildingActor: Player is not the editing player for this building!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerStateAthena) {
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team) {
			Log("ServerEditBuildingActor: Player is not on the same team as the building, cannot edit. Player hacking?");
			return;
		}
	}

	BuildingActorToEdit->SetEditingPlayer(nullptr);

	ABuildingSMActor* ReplacedBuildingActor = BuildingActorToEdit->ReplaceBuildingActor(
		EBuildingReplacementType::GetBRT_Edited(),
		NewBuildingClass.Class,
		BuildingActorToEdit->CurrentBuildingLevel,
		RotationIterations,
		bMirrored,
		this
	);

	if (!ReplacedBuildingActor) {
		Log("ServerEditBuildingActor: Failed to replace building actor!");
		return;
	}

	ReplacedBuildingActor->InitializeKismetSpawnedBuildingActor(ReplacedBuildingActor, this, true, BuildingActorToEdit);

	ReplacedBuildingActor->bPlayerPlaced = true;
	if (PlayerStateAthena) {
		ReplacedBuildingActor->Team = PlayerStateAthena->TeamIndex;
		ReplacedBuildingActor->TeamIndex = PlayerStateAthena->TeamIndex;
	}

	ReplacedBuildingActor->SetEditingPlayer(PlayerState);

	FFortItemEntry* EditToolEntry = WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
	if (!EditToolEntry) {
		Log("ServerEditBuildingActor: Failed to find edit tool in inventory!");
		return;
	}

	AFortWeap_EditingTool* EditingTool = MyFortPawn->FindWeapon(EditToolEntry->ItemGuid)->Cast<AFortWeap_EditingTool>();
	if (!EditingTool) {
		Log("ServerEditBuildingActor: Failed to find editing tool weapon instance!");
		return;
	}

	EditingTool->EditActor = ReplacedBuildingActor;
	EditingTool->ForceNetUpdate();
	EditingTool->OnRep_EditActor();
}

void AFortPlayerController::ServerEditBuildingActorHK(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored) {
	if (!This) return ServerEditBuildingActorOG(This, BuildingActorToEdit, NewBuildingClass, RotationIterations, bMirrored);

	This->ServerEditBuildingActor(BuildingActorToEdit, NewBuildingClass, RotationIterations, bMirrored);
}

void AFortPlayerController::ServerEndEditingBuildingActor(ABuildingSMActor* BuildingActorToStopEditing) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerEndEditingBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToStopEditing) {
		Log("ServerEndEditingBuildingActor: BuildingActorToStopEditing is null!");
		return;
	}

	AFortPlayerStateZone* PlayerState = PlayerState->Cast<AFortPlayerStateZone>();
	if (!PlayerState) {
		Log("ServerEndEditingBuildingActor: PlayerState is null or not AFortPlayerStateZone!");
		return;
	}

	AFortPawn* MyFortPawn = MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerEndEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	if (BuildingActorToStopEditing->EditingPlayer != PlayerState) {
		Log("ServerEndEditingBuildingActor: Player is not the editing player for this building!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerStateAthena) {
		if (PlayerStateAthena->TeamIndex != BuildingActorToStopEditing->Team) {
			Log("ServerEndEditingBuildingActor: Player is not on the same team as the building, cannot edit. Player hacking?");
			return;
		}
	}

	BuildingActorToStopEditing->SetEditingPlayer(nullptr);

	FFortItemEntry* EditToolEntry = WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
	if (!EditToolEntry) {
		Log("ServerEndEditingBuildingActor: Failed to find edit tool in inventory!");
		return;
	}

	AFortWeap_EditingTool* EditingTool = MyFortPawn->FindWeapon(EditToolEntry->ItemGuid)->Cast<AFortWeap_EditingTool>();
	if (!EditingTool) {
		Log("ServerEndEditingBuildingActor: Failed to find editing tool weapon instance!");
		return;
	}

	EditingTool->EditActor = nullptr;
	EditingTool->ForceNetUpdate();
	EditingTool->OnRep_EditActor();
}

void AFortPlayerController::ServerEndEditingBuildingActorHK(AFortPlayerController* This, ABuildingSMActor* BuildingActorToStopEditing) {
	if (!This) return ServerEndEditingBuildingActorOG(This, BuildingActorToStopEditing);

	This->ServerEndEditingBuildingActor(BuildingActorToStopEditing);
}

void AFortPlayerController::ServerAttemptInteract(AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType) {
	ServerAttemptInteractOG(this, ReceivingActor, InteractComponent, InteractType);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerAttemptInteract: World is null!");
		return;
	}

	//Log("ReceivingActor: " + ReceivingActor->GetFullName());
}

void AFortPlayerController::ServerAttemptInteractHK(AFortPlayerController* This, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType) {
	if (!This) return ServerAttemptInteractOG(This, ReceivingActor, InteractComponent, InteractType);

	This->ServerAttemptInteract(ReceivingActor, InteractComponent, InteractType);
}

void AFortPlayerController::ServerRemoveInventoryStateValue(FGuid& ItemGuid, uint8 StateValueType) {
	if (!WorldInventory) {
		Log("ServerRemoveInventoryStateValue: WorldInventory is null!");
		return;
	}

	FFortItemEntry* ItemEntry = FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("ServerRemoveInventoryStateValue: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	for (int32 i = 0; i < ItemEntry->StateValues.Num(); i++) {
		FFortItemEntryStateValue& StateValue = ItemEntry->StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateValueType) {
			ItemEntry->StateValues.RemoveAt(i);
		}
	}
}

void AFortPlayerController::ServerRemoveInventoryStateValueHK(AFortPlayerController* This, FGuid& ItemGuid, uint8 StateValueType) {
	if (!This) return ServerRemoveInventoryStateValueOG(This, ItemGuid, StateValueType);

	This->ServerRemoveInventoryStateValue(ItemGuid, StateValueType);
}

void AFortPlayerController::ServerSetInventoryStateValue(FGuid& ItemGuid, FFortItemEntryStateValue& StateValue) {
	Log("AFortPlayerController::ServerSetInventoryStateValue Called!");
	Log("PlayerController: " + GetFullName());

	if (!WorldInventory) {
		Log("ServerSetInventoryStateValue: WorldInventory is null!");
		return;
	}

	FFortItemEntry* ItemEntry = FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("ServerSetInventoryStateValue: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	ItemEntry->StateValues.Add(StateValue);
}

void AFortPlayerController::ServerSetInventoryStateValueHK(AFortPlayerController* This, FGuid& ItemGuid, FFortItemEntryStateValue& StateValue) {
	if (!This) return ServerSetInventoryStateValueOG(This, ItemGuid, StateValue);

	This->ServerSetInventoryStateValue(ItemGuid, StateValue);
}