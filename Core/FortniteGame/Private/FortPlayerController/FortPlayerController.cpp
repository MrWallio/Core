#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerController.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortManager/FortCheatManager.h"
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
#include "FortniteGame/Public/BuildingActor/BuildingItemCollectorActor.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/FortAbility/FortGameplayAbility.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/Athena/FortAthenaMapInfo.h"
#include "FortniteGame/Public/AI/FortAIController.h"
#include "FortniteGame/Public/AI/FortAIPawn.h"
#include "FortniteGame/Public/Athena/FortAthenaAircraft.h"

void AFortPlayerController::ClientForceProfileQuery()
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientForceProfileQuery");

	return Call(Func);
}

void AFortPlayerController::OnReadyToStartMatch(AFortPlayerController* This) {
	OnReadyToStartMatchOG(This);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::OnReadyToStartMatch: World is null!");
		return;
	}

	if (This->_HasQuickBars() && !This->QuickBars)
	{
		This->SpawnQuickBars();
		This->SetupQuickBars();
	}
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

		UClass* QuickBarsClass = AFortQuickBars::GetDefaultQuickBarsClass();

		if (IsUsingOldQuickBars()) {
			if (!QuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(QuickBarsClass, FVector(-280, 400, 3000), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					QuickBars = NewQuickBars->Cast<AFortQuickBars>();
					OnRep_QuickBar();

					Log("Spawned QuickBars: " + QuickBars->GetName().ToString());
				}
			}
		}
		else {
			if (!ClientQuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(QuickBarsClass, FVector(), FRotator(), this);
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

void AFortPlayerController::ServerCheat(AFortPlayerController* This, FString& Msg) {
	std::string Command = Msg.ToString();
	Log("ServerCheat (" + This->GetName().ToString() + "): [" + Command + "]");

	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bIsProd)
		return;
	
	UFortCheatManager* CheatManager = This->CheatManager->Cast<UFortCheatManager>();
	if (!CheatManager)
	{
		UFortCheatManager* NewCheatManager = (UFortCheatManager*)UGameplayStatics::SpawnObject(This->CheatClass.Get(), This);
		if (NewCheatManager) {
			This->CheatManager = NewCheatManager;
			CheatManager = NewCheatManager;
			This->ClientMessage("Spawned CheatManager: " + CheatManager->GetName().ToString());
		}
		else {
			This->ClientMessage("Failed to spawn CheatManager!");
			return;
		}
	}

	ServerCheatOG(This, Msg);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		This->ClientMessage("World is null!");
		return;
	}

	AFortGameMode* GameMode = World->AuthorityGameMode->Cast<AFortGameMode>();
	if (!GameMode) {
		This->ClientMessage("GameMode is null or not a FortGameMode!");
		return;
	}

	FCommandParser Parser(Command);

	if (Parser.IsCommand("Help"))
	{
		This->ClientMessage("=== Available Commands ===");
		This->ClientMessage("Params can also be passed by name: Command?Param=Value?bSomeFlag");
		This->ClientMessage("-- Items / Ammo --");
		This->ClientMessage("GiveItem <ItemDefinitionName> [Count] - Gives an item to the player's inventory.");
		This->ClientMessage("ForceGiveItem <ItemDefinitionName> [Count] - Forces an item into the inventory. Use this if GiveItem fails!");
		This->ClientMessage("SpawnPickup <ItemDefinitionName> [Count] - Spawns a pickup at the player's location.");
		This->ClientMessage("SetLoadedAmmo <LoadedAmmo> - Sets the loaded ammo of the currently equipped weapon.");
		This->ClientMessage("GiveAmmo [Amount] - Gives ammo for the currently equipped weapon.");
		This->ClientMessage("ClearEquippedItem - Removes the currently equipped item from the inventory.");
		This->ClientMessage("GetWeaponStats - Gets the stats of the currently equipped weapon.");
		This->ClientMessage("ServerExecuteInventoryItem <ItemGuid> - Executes an inventory item by its GUID.");
		This->ClientMessage("DumpInventory - Dumps inventory stats and items.");
		This->ClientMessage("-- Player --");
		This->ClientMessage("SetHealth <Health> / SetMaxHealth <MaxHealth> - Sets the player's health / max health.");
		This->ClientMessage("SetShield <Shield> / SetMaxShield <MaxShield> - Sets the player's shield / max shield.");
		This->ClientMessage("SetKillScore <NewScore> - Sets the player's kill score.");
		This->ClientMessage("TeleportToLocation <X> <Y> <Z> - Teleports the player to a specific location.");
		This->ClientMessage("DumpCurrentLocation - Dumps the player's current location and rotation.");
		This->ClientMessage("-- Pawns / Bots --");
		This->ClientMessage("DumpAllPawns - Lists every pawn in the world with its index.");
		This->ClientMessage("PossessPawnByIndex <PawnIndex> - Possesses a pawn by its index (see DumpAllPawns).");
		This->ClientMessage("PossessPawnByName <PawnName> - Possesses a pawn by name (case-insensitive, substring).");
		This->ClientMessage("-- World / Actors --");
		This->ClientMessage("SpawnActor <ActorClassName> [bSetOwnerAsThis] - Spawns an actor at the player.");
		This->ClientMessage("DumpActorsWithClass <ClassName> - Lists all actors of a class with their locations.");
		This->ClientMessage("DumpAircrafts - Dumps the aircrafts in the gamestate.");
		This->ClientMessage("DestroyTarget - Destroys the actor under the crosshair.");
		This->ClientMessage("-- QuickBars --");
		This->ClientMessage("SpawnQuickBars / DestroyQuickBars / DumpQuickBars - Manage the player's quickbars.");
		This->ClientMessage("-- Fun --");
		This->ClientMessage("LootRain [Count] [Radius] [TierGroup] - Rains loot-table drops down around you.");
		This->ClientMessage("SetGameSpeed [Multiplier] - Sets the game speed. 0.2 = slow motion, 5 = turbo, 1 = normal.");
		This->ClientMessage("DespawnAllBots - Removes every bot pawn from the world.");
		This->ClientMessage("TeleportAllToMe - Teleports every other pawn into a ring around you.");
		This->ClientMessage("SwapPlaces <PawnName> - Swaps locations with a pawn (case-insensitive, substring).");
		This->ClientMessage("LaunchPawn [PawnName] [ZVelocity] - Yeets a pawn (default: you) into the sky.");
		This->ClientMessage("SetScale <Multiplier> - Scales your pawn (body, weapon and collision).");
		This->ClientMessage("ScalePawn <PawnName> <Multiplier> - Scales any pawn. Giant bots!");
		This->ClientMessage("Goto <PawnName> - Teleports you to a pawn.");
		This->ClientMessage("DestroyBuildings [Radius] - Destroys every building around you.");
		This->ClientMessage("EmoteAll - Everyone in the world uses a random emote.");
		This->ClientMessage("EmoteAllSpecific [EmoteItemDefinitionName] - Everyone in the world uses a specific emote.");
		This->ClientMessage("EmotePlayerByName <PlayerName> [EmoteItemDefinitionName] - make a player use a specific emote.");
		This->ClientMessage("TogglePersonalVehicle - Toggle the personal vehicle.");
		return;
	}
	else if (Parser.IsCommand("GiveItem")) {
		std::string ItemDefName = Parser.GetArg("ItemDefinitionName", 0);
		int32 Count = Parser.GetArgInt("Count", 1, 1);

		if (ItemDefName.empty())
		{
			This->ClientMessage("Usage: GiveItem <ItemDefinitionName> [Count]");
			return;
		}

		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}

		UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
		if (!ItemObj) {
			This->ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		AFortPlayerPawn* Pawn = This->Pawn->Cast<AFortPlayerPawn>();
		if (!Pawn) {
			This->ClientMessage("Pawn is null!");
			return;
		}

		FVector FinalLoc = Pawn->K2_GetActorLocation();
		FVector ForwardVector = Pawn->GetActorForwardVector();

		ForwardVector.Z = 0.0f;

		float SizeSq = ForwardVector.SizeSquared();
		if (SizeSq > 0.0001f) {
			float Size = sqrtf(SizeSq);
			ForwardVector.X /= Size;
			ForwardVector.Y /= Size;
		}

		FinalLoc = FinalLoc + ForwardVector * 450.f;
		FinalLoc.Z += 50.f;

		const float RandomAngleVariation = ((float)rand() * 0.00109866634f) - 18.f;
		const float FinalAngle = RandomAngleVariation * 0.017453292519943295f;

		FinalLoc.X += cos(FinalAngle) * 100.f;
		FinalLoc.Y += sin(FinalAngle) * 100.f;

		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemDef,
			Count,
			FinalLoc,
			This->GetDropFinalLocation(),
			-1,
			true,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetOther(),
			EFortPickupSpawnSource::GetUnset(),
			This,
			false
		);

		if (Pickup) {
			FVector ZeroVector = { 0, 0, 0 };
			AFortPlayerPawn::ServerHandlePickup(Pawn, Pickup, Pickup->PickupLocationData.FlyTime, ZeroVector, true);
			This->ClientMessage("Given Item: (Item=" + ItemDef->GetName().ToString() + " Count=" + std::to_string(Count) + ")");
		}
		else {
			This->ClientMessage("Failed to spawn pickup for item: " + ItemDef->GetName().ToString() + " - try ForceGiveItem.");
		}
		return;
	}
	else if (Parser.IsCommand("ForceGiveItem")) {
		std::string ItemDefName = Parser.GetArg("ItemDefinitionName", 0);
		int32 Count = Parser.GetArgInt("Count", 1, 1);

		if (ItemDefName.empty())
		{
			This->ClientMessage("Usage: ForceGiveItem <ItemDefinitionName> [Count]");
			return;
		}

		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}

		UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
		if (!ItemObj) {
			This->ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		FFortItemEntry* AddedEntry = This->WorldInventory->AddItem(ItemDef, Count);
		if (AddedEntry) {
			This->ClientMessage("Force-gave item: (Item=" + ItemDef->GetName().ToString() + " Count=" + std::to_string(Count) + ")");
		}
		else {
			This->ClientMessage("Failed to add item: " + ItemDef->GetName().ToString());
		}
		return;
	}
	else if (Parser.IsCommand("SpawnPickup")) {
		std::string ItemDefName = Parser.GetArg("ItemDefinitionName", 0);
		int32 Count = Parser.GetArgInt("Count", 1, 1);

		if (ItemDefName.empty())
		{
			This->ClientMessage("Usage: SpawnPickup <ItemDefinitionName> [Count]");
			return;
		}

		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}

		UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
		if (!ItemObj) {
			This->ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		AFortPickup* SpawnedPickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemDef,
			Count,
			This->Pawn->K2_GetActorLocation(),
			This->GetDropFinalLocation(),
			-1,
			true,
			true,
			true,
			-1,
			EFortPickupSourceTypeFlag::GetPlayer(),
			EFortPickupSpawnSource::GetUnset(),
			This,
			false
		);

		if (SpawnedPickup) {
			This->ClientMessage("Spawned pickup: (Item=" + ItemDef->GetName().ToString() + " Count=" + std::to_string(Count) + ")");
		}
		else {
			This->ClientMessage("Failed to spawn pickup for item: " + ItemDef->GetName().ToString());
		}
		return;
	}
	else if (Parser.IsCommand("SetLoadedAmmo")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("LoadedAmmo"))
		{
			This->ClientMessage("Usage: SetLoadedAmmo <LoadedAmmo>");
			return;
		}

		int32 LoadedAmmo = Parser.GetArgInt("LoadedAmmo", 0, 30);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}
		AFortWeapon* CurrentWeapon = This->MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			This->ClientMessage("CurrentWeapon is null!");
			return;
		}

		FFortItemEntry* WeaponEntry = This->FindItemEntry(CurrentWeapon->ItemEntryGuid);
		if (!WeaponEntry) {
			This->ClientMessage("Weapon entry not found in inventory!");
			return;
		}

		WeaponEntry->LoadedAmmo = LoadedAmmo;
		This->WorldInventory->Update(WeaponEntry);
		This->ClientMessage("Set loaded ammo of current weapon to " + std::to_string(LoadedAmmo));
		return;
	}
	else if (Parser.IsCommand("GiveAmmo")) {
		int32 AmmoAmount = Parser.GetArgInt("Amount", 0, 30);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		AFortWeapon* CurrentWeapon = This->MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			This->ClientMessage("CurrentWeapon is null!");
			return;
		}

		if (!CurrentWeapon->WeaponData) {
			This->ClientMessage("WeaponData is null!");
			return;
		}

		UFortWorldItemDefinition* ItemDef = CurrentWeapon->WeaponData->Cast<UFortWorldItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("WeaponData is not a UFortWorldItemDefinition!");
			return;
		}

		UFortWorldItemDefinition* AmmoItemDef = ItemDef->GetAmmoWorldItemDefinition_BP();
		if (!AmmoItemDef) {
			This->ClientMessage("AmmoWorldItemDefinition is null!");
			return;
		}

		FFortItemEntry* AmmoEntry = This->FindItemEntry(AmmoItemDef);
		if (AmmoEntry) {
			AmmoEntry->Count += AmmoAmount;
			This->WorldInventory->Update(AmmoEntry);
			This->ClientMessage("Added " + AmmoItemDef->GetName().ToString() + " " + std::to_string(AmmoAmount) + " ammo to existing stack.");
		}
		else {
			AmmoEntry = This->WorldInventory->AddItem(AmmoItemDef, AmmoAmount);
			if (AmmoEntry) {
				This->ClientMessage("Added new ammo entry: " + AmmoItemDef->GetName().ToString() + " x" + std::to_string(AmmoAmount));
			}
			else {
				This->ClientMessage("Failed to add ammo item: " + AmmoItemDef->GetName().ToString());
			}
		}
		return;
	} else if (Parser.IsCommand("DumpInventory")) {
		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}
		This->ClientMessage("=== Inventory Dump ===");

		This->ClientMessage("Stats: ");
		This->ClientMessage("Inventory Capacity: " + std::to_string(This->WorldInventory->GetInventoryCapacity()));
		This->ClientMessage("Inventory Used: " + std::to_string(This->WorldInventory->GetInventoryUsed()));

		This->ClientMessage("Items: ");
		for (UFortWorldItem* ItemEntry : This->WorldInventory->Inventory.ItemInstances) {
			std::string ItemName = ItemEntry->ItemEntry.ItemDefinition ? ItemEntry->ItemEntry.ItemDefinition->GetName().ToString() : "Unknown";
			int32 Count = ItemEntry->ItemEntry.Count;
			std::string FormattedGuid = ItemEntry->ItemEntry.ItemGuid.FormatGuid();

			This->ClientMessage("Item: " + ItemName + ", Count: " + std::to_string(Count) + ", GUID: " + FormattedGuid);
		}

		This->ClientMessage("=== End of Inventory Dump ===");
		return;
	}
	else if (Parser.IsCommand("SetHealth")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("Health"))
		{
			This->ClientMessage("Usage: SetHealth <Health>");
			return;
		}

		float Health = Parser.GetArgFloat("Health", 0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetHealth(Health);

		This->ClientMessage("Set health to " + std::to_string(Health));
		return;
	} else if (Parser.IsCommand("SetShield")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("Shield"))
		{
			This->ClientMessage("Usage: SetShield <Shield>");
			return;
		}

		float Shield = Parser.GetArgFloat("Shield", 0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetShield(Shield);

		This->ClientMessage("Set shield to " + std::to_string(Shield));
		return;
	} else if (Parser.IsCommand("SetMaxHealth")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("MaxHealth"))
		{
			This->ClientMessage("Usage: SetMaxHealth <MaxHealth>");
			return;
		}

		float MaxHealth = Parser.GetArgFloat("MaxHealth", 0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetMaxHealth(MaxHealth);

		This->ClientMessage("Set max health to " + std::to_string(MaxHealth));
		return;
	} else if (Parser.IsCommand("SetMaxShield")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("MaxShield"))
		{
			This->ClientMessage("Usage: SetMaxShield <MaxShield>");
			return;
		}

		float MaxShield = Parser.GetArgFloat("MaxShield", 0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetMaxShield(MaxShield);

		This->ClientMessage("Set max shield to " + std::to_string(MaxShield));
		return;
	}
	else if (Parser.IsCommand("SpawnActor")) {
		std::string ActorClassName = Parser.GetArg("ActorClassName", 0);
		bool bSetOwnerAsThis = Parser.GetArgBool("bSetOwnerAsThis", 1, false);

		if (ActorClassName.empty())
		{
			This->ClientMessage("Usage: SpawnActor <ActorClassName> [bSetOwnerAsThis]");
			return;
		}

		FVector Location = This->Pawn ? This->Pawn->K2_GetActorLocation() : FVector();
		FRotator Rotation = This->Pawn ? This->Pawn->K2_GetActorRotation() : FRotator();

		UObject* ActorClassObj = Utils::GetObjectFromString(ActorClassName, EClassCastFlags::CASTCLASS_UClass);
		if (!ActorClassObj) {
			This->ClientMessage("Actor class not found: " + ActorClassName);
			return;
		}

		UClass* ActorClass = ActorClassObj->Cast<UClass>();
		if (!ActorClass) {
			This->ClientMessage("Object is not a UClass: " + ActorClassObj->GetName().ToString());
			return;
		}

		AActor* NewActor = World->SpawnActor(ActorClass, Location, Rotation, bSetOwnerAsThis ? This : nullptr);
		if (NewActor) {
			This->ClientMessage("Spawned actor: " + NewActor->GetName().ToString());
		}
		else {
			This->ClientMessage("Failed to spawn actor of class: " + ActorClass->GetName().ToString());
		}
		return;
	}
	else if (Parser.IsCommand("ClearEquippedItem")) {
		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		if (!This->MyFortPawn->CurrentWeapon) {
			This->ClientMessage("No currently equipped item to clear.");
			return;
		}

		This->WorldInventory->RemoveItem(This->MyFortPawn->CurrentWeapon->ItemEntryGuid);
		This->ClientMessage("Cleared currently equipped item.");
		return;
	}
	else if (Parser.IsCommand("GetWeaponStats")) {
		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}
		AFortWeapon* CurrentWeapon = This->MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			This->ClientMessage("CurrentWeapon is null!");
			return;
		}
		UFortWeaponItemDefinition* WeaponDef = CurrentWeapon->WeaponData;
		if (!WeaponDef) {
			This->ClientMessage("WeaponData is null!");
			return;
		}

		This->ClientMessage("=== Weapon Stats ===");
		This->ClientMessage("Weapon Name: " + WeaponDef->GetName().ToString());
		This->ClientMessage("ClipSize: " + std::to_string(WeaponDef->GetClipSize()));
		This->ClientMessage("Durability: " + std::to_string(WeaponDef->GetDurability()));
		This->ClientMessage("=== End of Weapon Stats ===");
		return;
	}
	else if (Parser.IsCommand("DestroyTarget"))
	{
		CheatManager->DestroyTarget();
		This->ClientMessage("Destroyed target.");
		return;
	}
	else if (Parser.IsCommand("DumpActorsWithClass")) {
		std::string ActorClassName = Parser.GetArg("ClassName", 0);

		if (ActorClassName.empty())
		{
			This->ClientMessage("Usage: DumpActorsWithClass <ClassName>");
			return;
		}

		UObject* ActorClassObj = Utils::GetObjectFromString(ActorClassName, EClassCastFlags::CASTCLASS_UClass);
		if (!ActorClassObj) {
			This->ClientMessage("Actor class not found: " + ActorClassName);
			return;
		}

		UClass* ActorClass = ActorClassObj->Cast<UClass>();
		if (!ActorClass) {
			This->ClientMessage("Object is not a UClass: " + ActorClassObj->GetName().ToString());
			return;
		}

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, ActorClass, &FoundActors);

		This->ClientMessage("Found " + std::to_string(FoundActors.Num()) + " actors of class: " + ActorClass->GetName().ToString());
		for (int32 i = 0; i < FoundActors.Num(); i++) {
			AActor* Actor = FoundActors[i];
			if (!Actor)
				continue;

			FVector ActorLocation = Actor->K2_GetActorLocation();
			This->ClientMessage("[" + std::to_string(i) + "] " + Actor->GetName().ToString()
				+ " @ X=" + std::to_string(ActorLocation.X) + " Y=" + std::to_string(ActorLocation.Y) + " Z=" + std::to_string(ActorLocation.Z));
		}
		return;
	}
	else if (Parser.IsCommand("TeleportToLocation")) {
		bool bHasNamedLocation = Parser.HasNamedArg("X") && Parser.HasNamedArg("Y") && Parser.HasNamedArg("Z");
		if (Parser.GetArgCount() < 3 && !bHasNamedLocation)
		{
			This->ClientMessage("Usage: TeleportToLocation <X> <Y> <Z>");
			return;
		}

		float X = Parser.GetArgFloat("X", 0, 0.0f);
		float Y = Parser.GetArgFloat("Y", 1, 0.0f);
		float Z = Parser.GetArgFloat("Z", 2, 0.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		FVector NewLocation(X, Y, Z);

		FHitResult HitResult;
		This->MyFortPawn->K2_SetActorLocation(NewLocation, false, &HitResult, true);
		This->ClientMessage("Teleported to location: X=" + std::to_string(X) + " Y=" + std::to_string(Y) + " Z=" + std::to_string(Z));
		
		return;
	}
	else if (Parser.IsCommand("DumpCurrentLocation")) {
		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		FVector CurrentLocation = This->MyFortPawn->K2_GetActorLocation();
		FRotator CurrentRotation = This->MyFortPawn->K2_GetActorRotation();

		This->ClientMessage("Location: X=" + std::to_string(CurrentLocation.X) + " Y=" + std::to_string(CurrentLocation.Y) + " Z=" + std::to_string(CurrentLocation.Z));
		This->ClientMessage("Rotation: Pitch=" + std::to_string(CurrentRotation.Pitch) + " Yaw=" + std::to_string(CurrentRotation.Yaw));

		return;
	}
	else if (Parser.IsCommand("SpawnQuickBars")) {
		This->SpawnQuickBars();
		This->SetupQuickBars();
		This->ClientMessage("Spawned QuickBars.");

		return;
	}
	else if (Parser.IsCommand("DestroyQuickBars")) {
		if (This->QuickBars) {
			This->QuickBars->K2_DestroyActor();
			This->QuickBars = nullptr;
			This->ForceNetUpdate();
			This->ClientMessage("Destroyed QuickBars.");
		}
		else if (This->ClientQuickBars) {
			This->ClientQuickBars->K2_DestroyActor();
			This->ClientQuickBars = nullptr;
			This->ForceNetUpdate();
			This->ClientMessage("Destroyed ClientQuickBars.");
		}
		else {
			This->ClientMessage("No QuickBars to destroy.");
		}

		return;
		}
	else if (Parser.IsCommand("DumpQuickBars")) {
		if (This->QuickBars) {
			This->ClientMessage("QuickBars: " + This->QuickBars->GetName().ToString() + " @ 0x" + std::format("{:X}", (uintptr_t)This->QuickBars));
		}
		else if (This->ClientQuickBars) {
			This->ClientMessage("ClientQuickBars: " + This->ClientQuickBars->GetName().ToString() + " @ 0x" + std::format("{:X}", (uintptr_t)This->ClientQuickBars));
		}
		else {
			This->ClientMessage("No QuickBars to dump.");
		}

		return;
	}
	else if (Parser.IsCommand("ServerExecuteInventoryItem")) {
		std::string GuidA = Parser.GetArg("ItemGuid", 0);

		if (GuidA.empty())
		{
			This->ClientMessage("Usage: ServerExecuteInventoryItem <ItemGuid>");
			return;
		}

		FGuid ItemGuid = FGuid::ParseGUID(GuidA);

		if (!This->FindItemInstance(ItemGuid)) {
			This->ClientMessage("No inventory item with GUID: " + GuidA + " (see DumpInventory)");
			return;
		}

		This->ServerExecuteInventoryItem(This, ItemGuid);
		This->ClientMessage("Executed inventory item: " + GuidA);

		return;
		}
	else if (Parser.IsCommand("PossessPawnByIndex")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("PawnIndex"))
		{
			This->ClientMessage("Usage: PossessPawnByIndex <PawnIndex>");
			return;
		}

		int32 PawnIndex = Parser.GetArgInt("PawnIndex", 0, 0);

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		if (PawnIndex < 0 || PawnIndex >= Pawns.Num()) {
			This->ClientMessage("Invalid PawnIndex. Must be between 0 and " + std::to_string(Pawns.Num() - 1));
			return;
		}

		APawn* TargetPawn = (APawn*)Pawns[PawnIndex];
		if (!TargetPawn) {
			This->ClientMessage("TargetPawn is null!");
			return;
		}

		if (TargetPawn == This->K2_GetPawn()) {
			This->ClientMessage("Already possessing that pawn.");
			return;
		}

		if (TargetPawn->Controller && TargetPawn->Controller != This) {
			This->ClientMessage("Warning: pawn is currently controlled by " + TargetPawn->Controller->GetName().ToString() + " -- taking it over.");
		}

		APawn* PreviousPawn = This->K2_GetPawn();

		This->PossessVFT(TargetPawn);

		This->ForceNetUpdate();
		TargetPawn->ForceNetUpdate();

		This->ClientMessage("Possessed pawn: " + TargetPawn->GetName().ToString() + " (" + TargetPawn->GetClass()->GetName().ToString() + ") at index " + std::to_string(PawnIndex));
		if (PreviousPawn) {
			This->ClientMessage("Left behind: " + PreviousPawn->GetName().ToString());
			if (PreviousPawn->Controller) {
				Log("PossessPawnByIndex: previous pawn " + PreviousPawn->GetName().ToString() + " still has a Controller (" + PreviousPawn->Controller->GetName().ToString() + ") after possession!");
				This->ClientMessage("Warning: previous pawn did not fully detach.");
			}
		}

		return;
	}
	else if (Parser.IsCommand("PossessPawnByName")) {
		std::string PawnName = Parser.GetArg("PawnName", 0);

		if (PawnName.empty())
		{
			This->ClientMessage("Usage: PossessPawnByName <PawnName> (case-insensitive, matches substrings)");
			return;
		}

		std::string PawnNameLower = Utils::StringToLower(PawnName);

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		APawn* ExactMatch = nullptr;
		TArray<APawn*> PartialMatches;

		for (AActor* Actor : Pawns) {
			if (!Actor)
				continue;

			std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());

			if (ActorNameLower == PawnNameLower) {
				ExactMatch = (APawn*)Actor;
				break;
			}

			if (ActorNameLower.find(PawnNameLower) != std::string::npos) {
				PartialMatches.Add((APawn*)Actor);
			}
		}

		APawn* TargetPawn = nullptr;

		if (ExactMatch) {
			TargetPawn = ExactMatch;
		}
		else if (PartialMatches.Num() == 1) {
			TargetPawn = PartialMatches[0];
		}
		else if (PartialMatches.Num() > 1) {
			This->ClientMessage("'" + PawnName + "' is ambiguous, matches " + std::to_string(PartialMatches.Num()) + " pawns:");
			for (int32 i = 0; i < PartialMatches.Num(); i++) {
				This->ClientMessage("  " + PartialMatches[i]->GetName().ToString());
			}
			This->ClientMessage("Be more specific.");
			return;
		}
		else {
			This->ClientMessage("Pawn with name '" + PawnName + "' not found.");
			return;
		}

		if (TargetPawn == This->K2_GetPawn()) {
			This->ClientMessage("Already possessing that pawn.");
			return;
		}

		if (TargetPawn->Controller && TargetPawn->Controller != This) {
			This->ClientMessage("Warning: pawn is currently controlled by " + TargetPawn->Controller->GetName().ToString() + " -- taking it over.");
		}

		APawn* PreviousPawn = This->K2_GetPawn();

		This->PossessVFT(TargetPawn);

		This->ForceNetUpdate();
		TargetPawn->ForceNetUpdate();

		This->ClientMessage("Possessed pawn: " + TargetPawn->GetName().ToString() + " (" + TargetPawn->GetClass()->GetName().ToString() + ")");
		if (PreviousPawn) {
			This->ClientMessage("Left behind: " + PreviousPawn->GetName().ToString());
			if (PreviousPawn->Controller) {
				Log("PossessPawnByName: previous pawn " + PreviousPawn->GetName().ToString() + " still has a Controller (" + PreviousPawn->Controller->GetName().ToString() + ") after possession!");
				This->ClientMessage("Warning: previous pawn did not fully detach.");
			}
		}

		return;
	}
	else if (Parser.IsCommand("DumpAllPawns")) {
		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		This->ClientMessage("Found " + std::to_string(Pawns.Num()) + " pawns in the world.");

		for (int32 i = 0; i < Pawns.Num(); ++i) {
			APawn* IndexedPawn = (APawn*)Pawns[i];
			if (!IndexedPawn)
				continue;

			std::string Line = "[" + std::to_string(i) + "] " + IndexedPawn->GetName().ToString() + " (" + IndexedPawn->GetClass()->GetName().ToString() + ")";
			if (IndexedPawn->Controller)
				Line += " - controlled by " + IndexedPawn->Controller->GetName().ToString();
			This->ClientMessage(Line);
		}

		return;
	}
	else if (Parser.IsCommand("SetKillScore")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("NewScore"))
		{
			This->ClientMessage("Usage: SetKillScore <NewScore>");
			return;
		}

		int32 NewScore = Parser.GetArgInt("NewScore", 0, 0);

		AFortPlayerStateAthena* PlayerState = This->PlayerState->Cast<AFortPlayerStateAthena>();
		if (!PlayerState) {
			This->ClientMessage("PlayerState is null or not AFortPlayerStateAthena!");
			return;
		}

		PlayerState->SetKillScore(NewScore);
		This->ClientMessage("Set kill score to " + std::to_string(NewScore));

		return;
	}
	else if (Parser.IsCommand("LootRain")) {
		if (!This->Pawn) {
			This->ClientMessage("Pawn is null!");
			return;
		}

		int32 Count = Parser.GetArgInt("Count", 0, 20);
		float Radius = Parser.GetArgFloat("Radius", 1, 600.0f);
		if (Count < 1) Count = 1;
		if (Count > 100) Count = 100;
		if (Radius < 100.0f) Radius = 100.0f;

		FName TierGroup;
		std::string TierGroupArg = Parser.GetArg("TierGroup", 2);
		if (!TierGroupArg.empty()) {
			TierGroup = UKismetStringLibrary::Conv_StringToName(TierGroupArg);
		}
		else if (GameMode->Cast<AFortGameModeAthena>()) {
			static FName Loot_AthenaTreasure = UKismetStringLibrary::Conv_StringToName("Loot_AthenaTreasure");
			TierGroup = Loot_AthenaTreasure;
		}
		else {
			static FName Loot_Treasure = UKismetStringLibrary::Conv_StringToName("Loot_Treasure");
			TierGroup = Loot_Treasure;
		}

		FVector Center = This->Pawn->K2_GetActorLocation();
		int32 Spawned = 0;

		for (int32 Roll = 0; Spawned < Count && Roll < Count * 4; Roll++) {
			TArray<FFortItemEntry> LootDrops;
			if (!UFortKismetLibrary::PickLootDrops(This, &LootDrops, TierGroup, 0, -1) || LootDrops.Num() == 0)
				break;

			for (int32 i = 0; i < LootDrops.Num() && Spawned < Count; i++) {
				FFortItemEntry& Entry = LootDrops.GetWithSize(i, FFortItemEntry::GetSize());
				if (!Entry.ItemDefinition)
					continue;

				const float Angle = ((float)rand() / (float)RAND_MAX) * 6.2831853f;
				const float Dist = ((float)rand() / (float)RAND_MAX) * Radius;

				FVector DropLoc = Center;
				DropLoc.X += cosf(Angle) * Dist;
				DropLoc.Y += sinf(Angle) * Dist;

				FVector AirLoc = DropLoc;
				AirLoc.Z += 400.0f + ((float)rand() / (float)RAND_MAX) * 400.0f;

				AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
					World,
					Entry.ItemDefinition,
					Entry.Count,
					AirLoc,
					DropLoc,
					-1,
					true,
					true,
					true,
					-1,
					EFortPickupSourceTypeFlag::GetOther(),
					EFortPickupSpawnSource::GetUnset(),
					This,
					false
				);

				if (!Pickup)
					continue;

				UFortWeaponItemDefinition* WeaponDef = Pickup->PrimaryPickupItemEntry.ItemDefinition->Cast<UFortWeaponItemDefinition>();
				if (WeaponDef) {
					int32 Level = Pickup->PrimaryPickupItemEntry.Level;
					Pickup->PrimaryPickupItemEntry.LoadedAmmo = WeaponDef->GetClipSize(Level);
					Pickup->PrimaryPickupItemEntry.Durability = WeaponDef->GetDurability(Level);
					Pickup->PrimaryPickupItemEntry.bIsDirty = true;
					Pickup->PrimaryPickupItemEntry.ReplicationKey++;
					Pickup->OnRep_PrimaryPickupItemEntry();
				}

				Spawned++;
			}
		}

		if (Spawned > 0)
			This->ClientMessage("Rained " + std::to_string(Spawned) + " items from '" + TierGroup.ToString().ToString() + "'.");
		else
			This->ClientMessage("PickLootDrops found nothing for tier group '" + TierGroup.ToString().ToString() + "'.");
		return;
	}
	else if (Parser.IsCommand("SetGameSpeed")) {
		float Speed = Parser.GetArgFloat("Multiplier", 0, 1.0f);
		if (Speed < 0.05f) Speed = 0.05f;
		if (Speed > 10.0f) Speed = 10.0f;

		AWorldSettings* WorldSettings = World->GetWorldSettings();
		if (!WorldSettings) {
			This->ClientMessage("WorldSettings is null!");
			return;
		}

		WorldSettings->TimeDilation = Speed;
		WorldSettings->ForceNetUpdate();

		if (Speed == 1.0f)
			This->ClientMessage("Game speed back to normal.");
		else
			This->ClientMessage("Game speed set to x" + std::to_string(Speed) + ". Use SetGameSpeed 1 to reset.");
		return;
	}
	else if (Parser.IsCommand("DespawnAllBots")) {
		int32 Removed = 0;

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		for (AActor* Actor : Pawns) {
			APawn* TargetPawn = (APawn*)Actor;
			if (!TargetPawn || TargetPawn == This->K2_GetPawn())
				continue;

			if (!TargetPawn->IsBotControlled())
				continue;

			TargetPawn->K2_DestroyActor();
			Removed++;
		}

		This->ClientMessage("Despawned " + std::to_string(Removed) + " bot pawns.");
		return;
	}
	else if (Parser.IsCommand("TeleportAllToMe")) {
		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		FVector Center = This->MyFortPawn->K2_GetActorLocation();
		int32 Moved = 0;
		for (AActor* Actor : Pawns) {
			APawn* TargetPawn = (APawn*)Actor;
			if (!TargetPawn || TargetPawn == This->MyFortPawn)
				continue;

			const float Angle = (Moved % 8) * 0.785398f;
			const float Ring = 300.0f + 150.0f * (Moved / 8);

			FVector NewLocation = Center;
			NewLocation.X += cosf(Angle) * Ring;
			NewLocation.Y += sinf(Angle) * Ring;
			NewLocation.Z += 50.0f;

			FHitResult HitResult;
			TargetPawn->K2_SetActorLocation(NewLocation, false, &HitResult, true);
			Moved++;
		}

		This->ClientMessage("Teleported " + std::to_string(Moved) + " pawns to you.");
		return;
	}
	else if (Parser.IsCommand("SwapPlaces")) {
		std::string PawnName = Parser.GetArg("PawnName", 0);

		if (PawnName.empty())
		{
			This->ClientMessage("Usage: SwapPlaces <PawnName> (case-insensitive, matches substrings)");
			return;
		}

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		std::string PawnNameLower = Utils::StringToLower(PawnName);

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		APawn* ExactMatch = nullptr;
		TArray<APawn*> PartialMatches;

		for (AActor* Actor : Pawns) {
			if (!Actor || Actor == This->MyFortPawn)
				continue;

			std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());

			if (ActorNameLower == PawnNameLower) {
				ExactMatch = (APawn*)Actor;
				break;
			}

			if (ActorNameLower.find(PawnNameLower) != std::string::npos) {
				PartialMatches.Add((APawn*)Actor);
			}
		}

		APawn* TargetPawn = nullptr;

		if (ExactMatch) {
			TargetPawn = ExactMatch;
		}
		else if (PartialMatches.Num() == 1) {
			TargetPawn = PartialMatches[0];
		}
		else if (PartialMatches.Num() > 1) {
			This->ClientMessage("'" + PawnName + "' is ambiguous, matches " + std::to_string(PartialMatches.Num()) + " pawns:");
			for (int32 i = 0; i < PartialMatches.Num(); i++) {
				This->ClientMessage("  " + PartialMatches[i]->GetName().ToString());
			}
			This->ClientMessage("Be more specific.");
			return;
		}
		else {
			This->ClientMessage("Pawn with name '" + PawnName + "' not found.");
			return;
		}

		FVector MyLocation = This->MyFortPawn->K2_GetActorLocation();
		FVector TheirLocation = TargetPawn->K2_GetActorLocation();

		FHitResult HitResult;
		This->MyFortPawn->K2_SetActorLocation(TheirLocation, false, &HitResult, true);
		TargetPawn->K2_SetActorLocation(MyLocation, false, &HitResult, true);

		This->ClientMessage("Swapped places with " + TargetPawn->GetName().ToString());
		return;
	}
	else if (Parser.IsCommand("LaunchPawn")) {
		std::string PawnName;
		float ZVelocity = 3000.0f;

		if (Parser.GetArgCount() >= 1) {
			std::string FirstArg = Parser.GetArg(0);

			char* End = nullptr;
			float ParsedVelocity = strtof(FirstArg.c_str(), &End);
			if (End != FirstArg.c_str() && *End == '\0') {
				ZVelocity = ParsedVelocity;
			}
			else {
				PawnName = FirstArg;
				ZVelocity = Parser.GetArgFloat(1, 3000.0f);
			}
		}

		if (Parser.HasNamedArg("PawnName"))
			PawnName = Parser.GetNamedArg("PawnName");
		if (Parser.HasNamedArg("ZVelocity"))
			ZVelocity = Parser.GetArgFloat("ZVelocity", 1, 3000.0f);

		APawn* TargetPawn = This->K2_GetPawn();

		if (!PawnName.empty()) {
			std::string PawnNameLower = Utils::StringToLower(PawnName);

			TArray<AActor*> Pawns;
			UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

			TargetPawn = nullptr;
			for (AActor* Actor : Pawns) {
				if (!Actor)
					continue;

				std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());
				if (ActorNameLower == PawnNameLower || ActorNameLower.find(PawnNameLower) != std::string::npos) {
					TargetPawn = (APawn*)Actor;
					break;
				}
			}

			if (!TargetPawn) {
				This->ClientMessage("Pawn with name '" + PawnName + "' not found.");
				return;
			}
		}

		if (!TargetPawn) {
			This->ClientMessage("No pawn to launch!");
			return;
		}

		ACharacter* TargetCharacter = TargetPawn->Cast<ACharacter>();
		if (!TargetCharacter) {
			This->ClientMessage(TargetPawn->GetName().ToString() + " is not a Character, cannot launch it.");
			return;
		}

		TargetCharacter->LaunchCharacter(FVector(0.0f, 0.0f, ZVelocity), false, true);
		This->ClientMessage("Launched " + TargetCharacter->GetName().ToString() + " with ZVelocity " + std::to_string(ZVelocity) + ".");
		return;
	}
	else if (Parser.IsCommand("SetScale")) {
		if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("Multiplier"))
		{
			This->ClientMessage("Usage: SetScale <Multiplier>");
			return;
		}

		float Scale = Parser.GetArgFloat("Multiplier", 0, 1.0f);
		if (Scale < 0.1f) Scale = 0.1f;
		if (Scale > 10.0f) Scale = 10.0f;

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		UCapsuleComponent* Capsule = This->MyFortPawn->GetCapsuleComponent();
		if (!Capsule) {
			This->ClientMessage("Pawn has no capsule component!");
			return;
		}

		Capsule->SetIsReplicated(true);
		This->MyFortPawn->SetActorScale3D(FVector(Scale, Scale, Scale));
		This->MyFortPawn->ForceNetUpdate();

		This->ClientMessage("Set scale to x" + std::to_string(Scale) + ".");
		return;
	}
	else if (Parser.IsCommand("ScalePawn")) {
		std::string PawnName = Parser.GetArg("PawnName", 0);
		bool bHasMultiplier = Parser.GetArgCount() >= 2 || Parser.HasNamedArg("Multiplier");

		if (PawnName.empty() || !bHasMultiplier)
		{
			This->ClientMessage("Usage: ScalePawn <PawnName> <Multiplier> (case-insensitive, matches substrings)");
			return;
		}

		float Scale = Parser.GetArgFloat("Multiplier", 1, 1.0f);
		if (Scale < 0.1f) Scale = 0.1f;
		if (Scale > 10.0f) Scale = 10.0f;

		std::string PawnNameLower = Utils::StringToLower(PawnName);

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		APawn* TargetPawn = nullptr;
		for (AActor* Actor : Pawns) {
			if (!Actor)
				continue;

			std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());
			if (ActorNameLower == PawnNameLower || ActorNameLower.find(PawnNameLower) != std::string::npos) {
				TargetPawn = (APawn*)Actor;
				break;
			}
		}

		if (!TargetPawn) {
			This->ClientMessage("Pawn with name '" + PawnName + "' not found.");
			return;
		}

		ACharacter* TargetCharacter = TargetPawn->Cast<ACharacter>();
		if (!TargetCharacter) {
			This->ClientMessage(TargetPawn->GetName().ToString() + " is not a Character, cannot scale it.");
			return;
		}

		UCapsuleComponent* Capsule = TargetCharacter->GetCapsuleComponent();
		if (Capsule)
			Capsule->SetIsReplicated(true);

		TargetCharacter->SetActorScale3D(FVector(Scale, Scale, Scale));
		TargetCharacter->ForceNetUpdate();

		This->ClientMessage("Scaled " + TargetCharacter->GetName().ToString() + " to x" + std::to_string(Scale));
		return;
	}
	else if (Parser.IsCommand("Goto")) {
		std::string PawnName = Parser.GetArg("PawnName", 0);

		if (PawnName.empty())
		{
			This->ClientMessage("Usage: Goto <PawnName> (case-insensitive, matches substrings)");
			return;
		}

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		std::string PawnNameLower = Utils::StringToLower(PawnName);

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		APawn* TargetPawn = nullptr;
		for (AActor* Actor : Pawns) {
			if (!Actor)
				continue;

			std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());
			if (ActorNameLower == PawnNameLower || ActorNameLower.find(PawnNameLower) != std::string::npos) {
				TargetPawn = (APawn*)Actor;
				break;
			}
		}

		if (!TargetPawn) {
			This->ClientMessage("Pawn with name '" + PawnName + "' not found.");
			return;
		}

		if (TargetPawn == This->MyFortPawn) {
			This->ClientMessage("That's you.");
			return;
		}

		FVector TargetLocation = TargetPawn->K2_GetActorLocation();
		TargetLocation.X += 150.0f;
		TargetLocation.Z += 50.0f;

		FHitResult HitResult;
		This->MyFortPawn->K2_SetActorLocation(TargetLocation, false, &HitResult, true);
		This->ClientMessage("Teleported to " + TargetPawn->GetName().ToString());
		return;
	}
	else if (Parser.IsCommand("DestroyBuildings")) {
		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		float Radius = Parser.GetArgFloat("Radius", 0, 2000.0f);
		if (Radius < 500.0f) Radius = 500.0f;
		if (Radius > 10000.0f) Radius = 10000.0f;

		TArray<AActor*> Buildings;
		UGameplayStatics::GetAllActorsOfClass(World, ABuildingActor::StaticClass(), &Buildings);

		int32 Destroyed = 0;
		for (AActor* Building : Buildings) {
			if (!Building)
				continue;

			if (This->MyFortPawn->GetDistanceTo(Building) > Radius)
				continue;

			Building->K2_DestroyActor();
			Destroyed++;
		}

		This->ClientMessage("Destroyed " + std::to_string(Destroyed) + " buildings within " + std::to_string((int32)Radius) + " units.");
		return;
	}
	else if (Parser.IsCommand("EmoteAll")) {
		TArray<UObject*> Emotes = FUObjectArray::GetObjectsOfClass(UFortMontageItemDefinitionBase::StaticClass());
		if (Emotes.Num() == 0) {
			This->ClientMessage("No emotes are loaded!");	
			return;
		}

		TArray<AActor*> Controllers;
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &Controllers);

		int32 Dancing = 0;
		for (AActor* Actor : Controllers) {
			AFortPlayerController* Controller = Actor ? Actor->Cast<AFortPlayerController>() : nullptr;
			if (!Controller)
				continue;

			ServerPlayEmoteItem(Controller, (UFortMontageItemDefinitionBase*)Emotes[rand() % Emotes.Num()], 0.0f);
			Dancing++;
		}

		This->ClientMessage(std::to_string(Dancing) + " pawns are emoting.");
		return;
	}
	else if (Parser.IsCommand("EmoteAllSpecific")) {
		std::string EmoteItemDefinitionName = Parser.GetArg("EmoteItemDefinitionName", 0);

		UFortMontageItemDefinitionBase* ChosenEmote = nullptr;
		if (!EmoteItemDefinitionName.empty()) {
			ChosenEmote = (UFortMontageItemDefinitionBase*)Utils::GetObjectFromString(EmoteItemDefinitionName, UFortMontageItemDefinitionBase::StaticClass());
			if (!ChosenEmote) {
				This->ClientMessage("EmoteItemDefinition not found: " + EmoteItemDefinitionName);
				return;
			}
		}
		else {
			TArray<UObject*> Emotes = FUObjectArray::GetObjectsOfClass(UFortMontageItemDefinitionBase::StaticClass());
			if (Emotes.Num() == 0) {
				This->ClientMessage("No emotes are loaded!");
				return;
			}

			ChosenEmote = (UFortMontageItemDefinitionBase*)Emotes[rand() % Emotes.Num()];
		}

		TArray<AActor*> Controllers;
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &Controllers);

		int32 Dancing = 0;
		for (AActor* Actor : Controllers) {
			AFortPlayerController* Controller = Actor ? Actor->Cast<AFortPlayerController>() : nullptr;
			if (!Controller)
				continue;

			ServerPlayEmoteItem(Controller, ChosenEmote, 0.0f);
			Dancing++;
		}

		This->ClientMessage(std::to_string(Dancing) + " pawns are emoting.");
		return;
	}
	else if (Parser.IsCommand("EmotePlayerByName")) {
		std::string PlayerName = Parser.GetArg("PlayerName", 0);

		if (PlayerName.empty())
		{
			This->ClientMessage("Usage: EmotePlayerByName <PlayerName> [EmoteItemDefinitionName]");
			return;
		}

		std::string EmoteItemDefinitionName = Parser.GetArg("EmoteItemDefinitionName", 0);

		UFortMontageItemDefinitionBase* ChosenEmote = nullptr;
		if (!EmoteItemDefinitionName.empty()) {
			ChosenEmote = (UFortMontageItemDefinitionBase*)Utils::GetObjectFromString(EmoteItemDefinitionName, UFortMontageItemDefinitionBase::StaticClass());
			if (!ChosenEmote) {
				This->ClientMessage("EmoteItemDefinition not found: " + EmoteItemDefinitionName);
				return;
			}
		}
		else {
			TArray<UObject*> Emotes = FUObjectArray::GetObjectsOfClass(UFortMontageItemDefinitionBase::StaticClass());
			if (Emotes.Num() == 0) {
				This->ClientMessage("No emotes are loaded!");
				return;
			}

			ChosenEmote = (UFortMontageItemDefinitionBase*)Emotes[rand() % Emotes.Num()];
		}

		TArray<AActor*> Controllers;
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &Controllers);

		for (AActor* Actor : Controllers) {
			AFortPlayerController* Controller = Actor ? Actor->Cast<AFortPlayerController>() : nullptr;
			if (!Controller)
				continue;

			AFortPlayerState* PlayerState = Controller->PlayerState->Cast<AFortPlayerState>();
			if (!PlayerState || PlayerState->GetPlayerName() != PlayerName)
				continue;

			ServerPlayEmoteItem(Controller, ChosenEmote, 0.0f);
			This->ClientMessage("Player: " + PlayerName + " is emoting!");
			return;
		}
	}
	else if (Parser.IsCommand("TogglePersonalVehicle")) {
		bool bOn = !This->IsPersonalVehicleActive();
		This->TogglePersonalVehicle(bOn);
		This->ClientMessage("Toggled Personal Vehicle!");
		return;
	}
	else if (Parser.IsCommand("DumpAircrafts")) {
		AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
		if (!GameState) {
			This->ClientMessage("GameState is null or not an AFortGameStateAthena!");
			return;
		}

		TArray<AFortAthenaAircraft*>& Aircrafts = GameState->Aircrafts;
		if (Aircrafts.IsEmpty()) {
			This->ClientMessage("Aircrafts array is empty!");
			return;
		}

		This->ClientMessage("===== Start Aircrafts Dump =====");
		for (AFortAthenaAircraft* Aircraft : Aircrafts) {
			This->ClientMessage("Aircraft: " + Aircraft->GetName().ToString());
		}
		This->ClientMessage("===== End Aircrafts Dump =====");
	}

	if (Version::Fortnite_Version >= 2.2) {
		UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), Msg, This);
	}
}

void AFortPlayerController::ServerExecuteInventoryItem(AFortPlayerController* This, FGuid& ItemGuid) {
	UFortWorldItem* ItemInstance = This->FindItemInstance(ItemGuid);
	if (!ItemInstance) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: ItemInstance not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	UFortWorldItemDefinition* ItemDef = ItemInstance->ItemEntry.ItemDefinition->Cast<UFortWorldItemDefinition>();
	if (!ItemDef) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: ItemDefinition is not a UFortWorldItemDefinition for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	AFortPawn* FortPawn = This->MyFortPawn;
	if (!FortPawn) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: MyFortPawn is null!");
		return;
	}

	//Log("ServerExecuteInventoryItem: " + ItemDef->GetName().ToString() + " (GUID: " + ItemGuid.FormatGuid() + ")");
	ItemDef->ServerExecute(ItemInstance, This);
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

	return Call(Func, BuildingSMActor, PotentialResourceType, PotentialResourceCount, bDestroyed, bJustHitWeakspot);
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

	FVector FinalLoc = MyFortPawn->K2_GetActorLocation();

	Log("AFortPlayerController::ServerAttemptInventoryDrop: Attempting to drop item with GUID: " + ItemGuid.FormatGuid() + ", Count: " + std::to_string(Count) + ", bTrash: " + std::to_string(bTrash));
	FFortItemEntry* ItemEntry = FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	AFortPickup* Pickup = nullptr;
	if (!bTrash) {
		Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry->ItemDefinition,
			Count,
			FinalLoc,
			GetDropFinalLocation(),
			-1,
			true,
			true,
			true,
			-1,
			EFortPickupSourceTypeFlag::GetPlayer(),
			EFortPickupSpawnSource::GetTossedByPlayer(),
			this,
			false
		);
		if (Pickup) {
			Pickup->PrimaryPickupItemEntry.LoadedAmmo = ItemEntry->LoadedAmmo;
			Pickup->PrimaryPickupItemEntry.Durability = ItemEntry->Durability;
			Pickup->PrimaryPickupItemEntry.bIsDirty = true;

			Pickup->PrimaryPickupItemEntry.ReplicationKey++;
			Pickup->OnRep_PrimaryPickupItemEntry();
		}
	}
	
	if (bTrash || Pickup) {
		WorldInventory->RemoveItem(ItemEntry->ItemGuid, Count);
	}
}

void AFortPlayerController::execServerAttemptInventoryDrop(AFortPlayerController* Context, FFrame& Stack) {
	static UFunction* ServerAttemptInventoryDropFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInventoryDrop");
	if (!ServerAttemptInventoryDropFn) {
		Log("AFortPlayerController::execServerAttemptInventoryDrop: Failed to find function!");
		return;
	}
	
	FGuid& ItemGuid = Stack.StepCompiledInRef<FGuid>();
	int32 Count = 0;
	bool bTrash = false;
	Stack.StepCompiledIn(&Count);
	
	for (auto& Param : ServerAttemptInventoryDropFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "bTrash") {
			Stack.StepCompiledIn(&bTrash);
		}
	}
	Stack.IncrementCode();

	Context->ServerAttemptInventoryDrop(ItemGuid, Count, bTrash);
}

void AFortPlayerController::ServerSpawnInventoryDrop(FGuid& ItemGuid, int32 Count) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerSpawnInventoryDrop: World is null!");
		return;
	}

	if (!MyFortPawn) {
		Log("AFortPlayerController::ServerSpawnInventoryDrop: MyFortPawn is null!");
		return;
	}

	FVector FinalLoc = MyFortPawn->K2_GetActorLocation();

	Log("AFortPlayerController::ServerSpawnInventoryDrop: Attempting to drop item with GUID: " + ItemGuid.FormatGuid() + ", Count: " + std::to_string(Count));
	FFortItemEntry* ItemEntry = FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("AFortPlayerController::ServerSpawnInventoryDrop: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
		World,
		ItemEntry->ItemDefinition,
		Count,
		FinalLoc,
		GetDropFinalLocation(),
		-1,
		true,
		true,
		true,
		-1,
		EFortPickupSourceTypeFlag::GetPlayer(),
		EFortPickupSpawnSource::GetTossedByPlayer(),
		this,
		false
	);
	if (Pickup) {
		Pickup->PrimaryPickupItemEntry.LoadedAmmo = ItemEntry->LoadedAmmo;
		Pickup->PrimaryPickupItemEntry.Durability = ItemEntry->Durability;
		Pickup->PrimaryPickupItemEntry.bIsDirty = true;

		Pickup->PrimaryPickupItemEntry.ReplicationKey++;
		Pickup->OnRep_PrimaryPickupItemEntry();

		WorldInventory->RemoveItem(ItemEntry->ItemGuid, Count);
	}
}

void AFortPlayerController::execServerSpawnInventoryDrop(AFortPlayerController* Context, FFrame& Stack) {
	FGuid& ItemGuid = Stack.StepCompiledInRef<FGuid>();
	int32 Count = 0;
	Stack.StepCompiledIn(&Count);
	Stack.IncrementCode();

	Context->ServerSpawnInventoryDrop(ItemGuid, Count);
}

void AFortPlayerController::ClientForceUpdateQuickbar(uint8 QuickbarToRefresh)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientForceUpdateQuickbar");

	return Call(Func, QuickbarToRefresh);
}

void AFortPlayerController::OnRep_QuickBar()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_QuickBar");

	Call(Func);
}

bool AFortPlayerController::IsUsingOldQuickBars() {
	static uintptr_t ClientQuickBarsOffset = StaticClass()->GetPropertyOffset("ClientQuickBars");

	return ClientQuickBarsOffset == -1;
}

void AFortPlayerController::ServerClientPawnLoaded(AFortPlayerController* This, bool bIsPawnLoaded)
{
	ServerClientPawnLoadedOG(This, bIsPawnLoaded);
	AFortPlayerState* FortPlayerState = This->PlayerState ? This->PlayerState->Cast<AFortPlayerState>() : nullptr;
	if (!FortPlayerState) {
		Log("ServerAcknowledgePossession: PlayerState is null or not AFortPlayerStateZone");
		return;
	}

	AFortPlayerPawn* MyFortPawn = This->MyFortPawn;

	AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>();
	AFortPlayerStateAthena* FortPSAthena = FortPlayerState->Cast<AFortPlayerStateAthena>();

	if (bIsPawnLoaded) {
		FortPlayerState->ApplyCharacterCustomization(MyFortPawn);

		if (FortPSAthena) {
			FortPSAthena->SetKillScore(FortPSAthena->KillScore);
		}
	}
}

bool AFortPlayerController::RemoveInventoryItem(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoveFromQuickBars, bool bForceRemoval)
{
	static auto InterfaceOffset = StaticClass()->SuperStruct->PropertiesSize + (Version::Engine_Version >= 4.27 ? 16 : 8);
	AFortPlayerController* PlayerController = (AFortPlayerController*)(__int64(This) - InterfaceOffset); // this is so wierd
	if (!PlayerController) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	AFortInventory* Inventory = PlayerController->WorldInventory;
	if (!Inventory) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	if (!ItemGuid.IsValid()) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	FFortItemEntry* ItemEntry = Inventory->FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	UFortWorldItemDefinition* WorldItemDefinition = ItemEntry->ItemDefinition->Cast<UFortWorldItemDefinition>();

	bool bForceRemoveItem = bForceRemoveFromQuickBars || bForceRemoval;
	if (!bForceRemoveItem) {
		bool bShouldRemoveCount = ItemEntry->Count > Count;
		if (WorldItemDefinition && WorldItemDefinition->bPersistInInventoryWhenFinalStackEmpty) {
			bShouldRemoveCount = ItemEntry->Count >= Count;
		}

		if (bShouldRemoveCount) {
			ItemEntry->Count -= Count;
			return Inventory->Update(ItemEntry);
		}
		else {
			return Inventory->RemoveItem(ItemGuid, Count);
		}
	}
	else {
		return Inventory->RemoveItem(ItemGuid, Count);
	}
}

void AFortPlayerController::ServerCreateBuildingActorOld(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored, float SyncKey) {
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

	if (!This->CanAffordToPlaceBuildableClass(&BuildingClassData)) {
		return;
	}

	TArray<ABuildingActor*> BuildingsToRemove;
	uint8 OptionalAdjustment;
	if (UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(World, BuildingClassData.BuildingClass.Get(), BuildLoc, BuildRot, bMirrored, &BuildingsToRemove, &OptionalAdjustment)) {
		return;
	}

	for (ABuildingActor* BuildingToRemove : BuildingsToRemove) {
		if (BuildingToRemove) {
			BuildingToRemove->K2_DestroyActor();
		}
	}
	BuildingsToRemove.Free();

	ABuildingActor* BuildingActor = World->SpawnActorUnfinished(BuildingClassData.BuildingClass.Class, BuildLoc, BuildRot, This)->Cast<ABuildingActor>();
	if (!BuildingActor) {
		Log("ServerCreateBuildingActor: Failed to spawn building actor!");
		return;
	}
	BuildingActor->InitializeKismetSpawnedBuildingActor(BuildingActor, This, true, nullptr);
	World->FinishSpawnActor(BuildingActor, BuildLoc, BuildRot);

	BuildingActor->CurrentBuildingLevel = BuildingClassData.UpgradeLevel;
	BuildingActor->OnRep_CurrentBuildingLevel();

	ABuildingSMActor* BuildingSMActor = BuildingActor->Cast<ABuildingSMActor>();
	if (BuildingSMActor) {
		BuildingSMActor->bMirrored = bMirrored;
	}

	BuildingActor->bPlayerPlaced = true;

	AFortPlayerStateAthena* PlayerState = This->PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerState) {
		BuildingActor->Team = PlayerState->TeamIndex;
	}
	BuildingActor->TeamIndex = BuildingActor->Team;

	This->PayBuildableClassPlacementCost(&BuildingClassData);
}

bool AFortPlayerController::CanAffordToPlaceBuildableClass(FBuildingClassData* ClassToBuildData) {
	bool (*&CanAffordToPlaceBuildableClassInternal)(AFortPlayerController * This, FBuildingClassData * ClassToBuildData) = decltype(CanAffordToPlaceBuildableClassInternal)(VTable[Finder::FindAFortPlayerController_CanAffordToPlaceBuildableClassVFT()]);
	return CanAffordToPlaceBuildableClassInternal(this, ClassToBuildData);
}

int32 AFortPlayerController::PayBuildableClassPlacementCost(FBuildingClassData* ClassToBuildData) {
	int32(*&PayBuildableClassPlacementCostInternal)(AFortPlayerController * This, FBuildingClassData * ClassToBuildData) = decltype(PayBuildableClassPlacementCostInternal)(VTable[Finder::FindAFortPlayerController_PayBuildableClassPlacementCostVFT()]);
	return PayBuildableClassPlacementCostInternal(this, ClassToBuildData);
}

void AFortPlayerController::ServerBeginEditingBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerBeginEditingBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToEdit) {
		Log("ServerBeginEditingBuildingActor: BuildingActorToEdit is null!");
		return;
	}

	AFortPlayerStateZone* PlayerState = This->PlayerState->Cast<AFortPlayerStateZone>();
	if (!PlayerState) {
		Log("ServerBeginEditingBuildingActor: PlayerState is null or not AFortPlayerStateZone!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerStateAthena) {
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team && BuildingActorToEdit->bPlayerPlaced) {
			Log("ServerBeginEditingBuildingActor: Player is not on the same team as the building, cannot edit. Player hacking?");
			return;
		}
	}

	BuildingActorToEdit->SetEditingPlayer(PlayerState);

	AFortPawn* MyFortPawn = This->MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerBeginEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	if (!MyFortPawn->CurrentWeapon->Cast<AFortWeap_EditingTool>()) {
		FFortItemEntry* ItemEntry = This->WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
		if (ItemEntry) {
			This->ServerExecuteInventoryItem(This, ItemEntry->ItemGuid);
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
	EditingTool->OnRep_EditActor();
}

void AFortPlayerController::ServerEditBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored) {
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

	AFortPawn* MyFortPawn = This->MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerEndEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	AFortPlayerStateZone* PlayerState = This->PlayerState->Cast<AFortPlayerStateZone>();
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
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team && BuildingActorToEdit->bPlayerPlaced) {
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
		This
	);

	if (!ReplacedBuildingActor) {
		Log("ServerEditBuildingActor: Failed to replace building actor!");
		return;
	}

	ReplacedBuildingActor->bPlayerPlaced = true;
	if (PlayerStateAthena) {
		ReplacedBuildingActor->Team = PlayerStateAthena->TeamIndex;
		ReplacedBuildingActor->TeamIndex = PlayerStateAthena->TeamIndex;
	}

	if (Version::Fortnite_Version <= 1.72) {
		ServerEndEditingBuildingActor(This, ReplacedBuildingActor);
	}
}

void AFortPlayerController::ServerEndEditingBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToStopEditing) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerEndEditingBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToStopEditing) {
		Log("ServerEndEditingBuildingActor: BuildingActorToStopEditing is null!");
		return;
	}

	AFortPawn* MyFortPawn = This->MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerEndEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	BuildingActorToStopEditing->SetEditingPlayer(nullptr);

	FFortItemEntry* EditToolEntry = This->WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
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
	EditingTool->OnRep_EditActor();
}

void AFortPlayerController::ServerRemoveInventoryStateValue(AFortPlayerController* This, FGuid& ItemGuid, uint8 StateValueType) {
	if (!This->WorldInventory) {
		Log("ServerRemoveInventoryStateValue: WorldInventory is null!");
		return;
	}

	FFortItemEntry* ItemEntry = This->FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("ServerRemoveInventoryStateValue: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	bool bModified = false;
	for (int32 i = 0; i < ItemEntry->StateValues.Num(); i++) {
		FFortItemEntryStateValue& StateValue = ItemEntry->StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateValueType) {
			ItemEntry->StateValues.RemoveAt(i, FFortItemEntryStateValue::GetSize());
			bModified = true;
		}
	}

	if (bModified) {
		This->WorldInventory->Update(ItemEntry);
	}
}

void AFortPlayerController::ServerSetInventoryStateValue(AFortPlayerController* This, FGuid& ItemGuid, FFortItemEntryStateValue& StateValue) {
	Log("AFortPlayerController::ServerSetInventoryStateValue Called!");
	Log("PlayerController: " + This->GetFullName());

	if (!This->WorldInventory) {
		Log("ServerSetInventoryStateValue: WorldInventory is null!");
		return;
	}

	FFortItemEntry* ItemEntry = This->FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("ServerSetInventoryStateValue: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	ItemEntry->StateValues.Add(StateValue);
	This->WorldInventory->Update(ItemEntry);
}

UFortRegisteredPlayerInfo* AFortPlayerController::GetRegisteredPlayerInfo() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetRegisteredPlayerInfo");

	return const_cast<AFortPlayerController*>(this)->Call<UFortRegisteredPlayerInfo*>(Func);
}

UFortQuestManager* AFortPlayerController::GetQuestManager(ESubGame SubGame) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetQuestManager");

	return const_cast<AFortPlayerController*>(this)->Call<UFortQuestManager*>(Func, SubGame);
}

void AFortPlayerController::ServerRepairBuildingActor(ABuildingSMActor* BuildingActorToRepair) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerRepairBuildingActor: World is null!");
		return;
	}

	if (BuildingActorToRepair->EditingPlayer) {
		return;
	}

	if (int32 RepairCost = PayBuildingRepairCost(BuildingActorToRepair)) {
		BuildingActorToRepair->RepairBuilding(this, RepairCost);
	}
}

void AFortPlayerController::execServerRepairBuildingActor(AFortPlayerController* Context, FFrame& Stack) {
	struct FortPlayerController_ServerRepairBuildingActor
	{
	public:
		ABuildingSMActor* BuildingActorToRepair;
	};
	FortPlayerController_ServerRepairBuildingActor* Parms = (FortPlayerController_ServerRepairBuildingActor*)Stack.Locals;

	execServerRepairBuildingActorOG(Context, Stack);
	if (Version::Fortnite_Version >= 1.8 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		Context->ServerRepairBuildingActor(Parms->BuildingActorToRepair);
	}
}

int32 AFortPlayerController::PayBuildingRepairCost(ABuildingSMActor* BuildingToRepair) {
	int32(*PayBuildingRepairCostInternal)(AFortPlayerController * This, ABuildingSMActor * BuildingToRepair) = decltype(PayBuildingRepairCostInternal)(ImageBase + Finder::FindAFortPlayerController_PayBuildingRepairCost());
	return PayBuildingRepairCostInternal(this, BuildingToRepair);
}

void AFortPlayerController::ServerPlayEmoteItem(AFortPlayerController* This, UFortMontageItemDefinitionBase* EmoteAsset, float EmoteRandomNumber) {
	ServerPlayEmoteItemOG(This, EmoteAsset);
	if (Version::Fortnite_Version <= 1.82 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		return;
	}

	if (!EmoteAsset) {
		Log("ServerPlayEmoteItem: EmoteAsset is null!");
		return;
	}

	if (!This->MyFortPawn) {
		Log("ServerPlayEmoteItem: MyFortPawn is null!");
		return;
	}

	UFortAbilitySystemComponent* ASC = This->MyFortPawn->AbilitySystemComponent;
	if (!ASC) {
		Log("ServerPlayEmoteItem: AbilitySystemComponent is null!");
		return;
	}

	UFortGameplayAbility* EmoteAbility = nullptr;
	if (UAthenaSprayItemDefinition* SprayItemDef = EmoteAsset->Cast<UAthenaSprayItemDefinition>()) {
		UClass* Ability = StaticLoadObject<UClass>("/Game/Abilities/Sprays/GAB_Spray_Generic.GAB_Spray_Generic_C");
		if (Ability) {
			EmoteAbility = Ability->GetDefaultObj()->Cast<UFortGameplayAbility>();
		}
	}
	else if (UAthenaDanceItemDefinition* DanceItemDef = EmoteAsset->Cast<UAthenaDanceItemDefinition>()) {
		UClass* Ability = StaticLoadObject<UClass>("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");
		if (Ability) {
			EmoteAbility = Ability->GetDefaultObj()->Cast<UFortGameplayAbility>();
		}
	}

	if (EmoteAbility) {
		FGameplayAbilitySpec* EmoteAbilitySpec = FGameplayAbilitySpec::ConstructAbilitySpec(EmoteAbility, 1, -1, EmoteAsset);
		ASC->GiveAbilityAndActivateOnce(EmoteAbilitySpec, nullptr);
	}
}

void AFortPlayerController::ServerPlaySprayItem(AFortPlayerController* This, UAthenaSprayItemDefinition* SprayAsset) {
	if (!SprayAsset) {
		Log("ServerPlaySprayItem: SprayAsset is null!");
		return;
	}

	if (!This->MyFortPawn) {
		Log("ServerPlaySprayItem: MyFortPawn is null!");
		return;
	}

	UFortAbilitySystemComponent* ASC = This->MyFortPawn->AbilitySystemComponent;
	if (!ASC) {
		Log("ServerPlaySprayItem: AbilitySystemComponent is null!");
		return;
	}

	UFortGameplayAbility* SprayAbility = nullptr;
	if (UClass* Ability = StaticLoadObject<UClass>("/Game/Abilities/Sprays/GAB_Spray_Generic.GAB_Spray_Generic_C")) {
		SprayAbility = Ability->GetDefaultObj()->Cast<UFortGameplayAbility>();
	}

	if (SprayAbility) {
		FGameplayAbilitySpec* EmoteAbilitySpec = FGameplayAbilitySpec::ConstructAbilitySpec(SprayAbility, 1, -1, SprayAsset);
		ASC->GiveAbilityAndActivateOnce(EmoteAbilitySpec, nullptr);
	}
}

void AFortPlayerController::GetPlayerViewPoint(AFortPlayerController* This, FVector& out_Location, FRotator& out_Rotation) {
	GetPlayerViewPointOG(This, out_Location, out_Rotation);
	if (Version::Fortnite_Version <= 1.72) {
		return;
	}

	AActor* ViewTarget = This->GetViewTarget();
	AFortPlayerPawn* ViewTargetFortPawn = ViewTarget->Cast<AFortPlayerPawn>();

	if (ViewTarget) {
		This->GetActorEyesViewPoint(&out_Location, &out_Rotation);
	}
}

FUniqueNetIdRepl AFortPlayerController::GetGameAccountId() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetGameAccountId");

	return const_cast<AFortPlayerController*>(this)->Call<FUniqueNetIdRepl>(Func);
}

void AFortPlayerController::ServerAttemptInteract(AFortPlayerController* This, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType, UObject* OptionalObjectData) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerAttemptInteract: World is null!");
		return;
	}

	if (!ReceivingActor) {
		Log("AFortPlayerController::ServerAttemptInteract: ReceivingActor is null!");
		return;
	}

	UFortQuestManager* QuestManager = This->GetQuestManager(ESubGame::GetAthena());

	if (ABuildingItemCollectorActor* ItemCollector = ReceivingActor->Cast<ABuildingItemCollectorActor>()) {
		ItemCollector->ControllingPlayer = This;
		ItemCollector->bCurrentInteractionSuccess = ItemCollector->GrantOutput();
		ItemCollector->ControllingPlayer = nullptr;
	}

	//Log("ReceivingActor: " + ReceivingActor->GetName().ToString());
	ReceivingActor->ForceNetUpdate();
}

void AFortPlayerController::execServerAttemptInteract(AFortPlayerController* Context, FFrame& Stack) {
	struct FortPlayerController_ServerAttemptInteract
	{
	public:
		AActor* ReceivingActor;
		UPrimitiveComponent* InteractComponent;
		uint8 InteractType;
		UObject* OptionalObjectData;
	};
	FortPlayerController_ServerAttemptInteract* Parms = (FortPlayerController_ServerAttemptInteract*)Stack.Locals;

	ServerAttemptInteract(Context, Parms->ReceivingActor, Parms->InteractComponent, Parms->InteractType, Parms->OptionalObjectData);
	execServerAttemptInteractOG(Context, Stack);
}

FVector AFortPlayerController::GetDropFinalLocation()
{
	if (!MyFortPawn)
		return FVector{};

	FVector FinalLoc = MyFortPawn->K2_GetActorLocation();
	const FVector Forward = MyFortPawn->GetActorForwardVector();

	FinalLoc += Forward * 450.0f;
	FinalLoc.Z += 50.0f;

	const float AngleDeg = UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f);
	const float AngleRad = AngleDeg * 0.017453292519943295f;
	const float Radius = UKismetMathLibrary::RandomFloatInRange(0.0f, 100.0f);

	FinalLoc.X += std::cos(AngleRad) * Radius;
	FinalLoc.Y += std::sin(AngleRad) * Radius;

	return FinalLoc;
}

void AFortPlayerController::ClientExecuteInventoryItem(FGuid& ItemGuid, float Delay, bool bForceExecute, bool bActivateSlotAfterSettingFocused)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientExecuteInventoryItem");

	if (!Func) {
		return;
	}

	Call(Func, ItemGuid, Delay, bForceExecute, bActivateSlotAfterSettingFocused);
}

// gotta make this proper at some point
void AFortPlayerController::TogglePersonalVehicle(bool bOn) {
	Log("AFortPlayerController::TogglePersonalVehicle called with bOn = " + std::to_string(bOn));

	UFortAbilitySystemComponent* ASC = MyFortPawn ? MyFortPawn->AbilitySystemComponent : nullptr;
	if (!ASC) {
		Log("TogglePersonalVehicle: AbilitySystemComponent is null!");
		return;
	}

	UFortPersonalVehicleItemDefinition* PersonalVehicleDef = (UFortPersonalVehicleItemDefinition*)StaticLoadObject("/Game/Items/Hoverboard/VID_Hoverboard.VID_Hoverboard");
	if (!PersonalVehicleDef) {
		Log("TogglePersonalVehicle: Failed to load personal vehicle definition!");
		return;
	}

	UClass* PersonalVehicleAbilityClass = PersonalVehicleDef->PersonalVehicleAbility.Get();
	if (!PersonalVehicleAbilityClass) {
		Log("TogglePersonalVehicle: PersonalVehicleAbility is null!");
		return;
	}

	UFortGameplayAbility* PersonalVehicleAbility = PersonalVehicleAbilityClass->GetDefaultObj()->Cast<UFortGameplayAbility>();
	if (!PersonalVehicleAbility) {
		Log("TogglePersonalVehicle: Failed to get default object for personal vehicle ability!");
		return;
	}

	if (PersonalVehicleAbility) {
		if (bOn) {
			FGameplayAbilitySpec* PersonalVehicleAbilitySpec = FGameplayAbilitySpec::ConstructAbilitySpec(PersonalVehicleAbility, 1, -1, nullptr);
			ASC->GiveAbilityAndActivateOnce(PersonalVehicleAbilitySpec, nullptr);
		}
		else {
			ASC->EndAbility(PersonalVehicleAbility);
		}
	}
}

void AFortPlayerController::TogglePersonalVehicleHook(AFortPlayerController* This, bool bOn) {
	if (!This) {
		Log("TogglePersonalVehicleHook: This is null!");
		return;
	}

	if (AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>()) {
		FortPCAthena->TogglePersonalVehicle(bOn);
	}

	This->TogglePersonalVehicle(bOn);
}

void AFortPlayerController::execTogglePersonalVehicle(AFortPlayerController* Context, FFrame& Stack) {
	bool bOn = false;
	Stack.StepCompiledIn(&bOn);
	Stack.IncrementCode();

	TogglePersonalVehicleHook(Context, bOn);
}

bool AFortPlayerController::IsPersonalVehicleActive() const {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsPersonalVehicleActive");

	return const_cast<AFortPlayerController*>(this)->Call<bool>(Func);
}

bool AFortPlayerController::IsPersonalVehicleAvailable() const {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsPersonalVehicleAvailable");

	return const_cast<AFortPlayerController*>(this)->Call<bool>(Func);
}

bool AFortPlayerController::IsUsingPersonalVehicle() const {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsUsingPersonalVehicle");

	return const_cast<AFortPlayerController*>(this)->Call<bool>(Func);
}

void AFortPlayerController::ServerModifyStat(FName StatName, int32 Amount, uint8 ModType, bool bForceStatSave) {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerModifyStat");

	return Call(Func, StatName, Amount, ModType, bForceStatSave);
}

void AFortPlayerController::OnReadyToStartMatchVFT() {
	void (*&OnReadyToStartMatchInternal)(AFortPlayerController*) = decltype(OnReadyToStartMatchInternal)(VTable[Finder::FindAFortPlayerController_OnReadyToStartMatchVFT()]);
	return OnReadyToStartMatchInternal(this);
}