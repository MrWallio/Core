#pragma once
#include "pch.h"

#include "FortItemDefinition.h"
#include "FortniteGame/Public/FortEnums.h"

class UFortWorldItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortWorldItemDefinition);

	DefineUProperty(EWorldItemDropBehavior, DropBehavior);
	DefineBitfieldUProperty(bShouldShowItemToast);
	DefineBitfieldUProperty(bCanBeDropped);
	DefineBitfieldUProperty(bPersistInInventoryWhenFinalStackEmpty);
	DefineBitfieldUProperty(bAllowedToBeLockedInInventory);
	DefineBitfieldUProperty(bDropOnDeath);
	DefineBitfieldUProperty(bDropOnLogout);
	DefineBitfieldUProperty(bSupportsQuickbarFocus);
	DefineBitfieldUProperty(bSupportsQuickbarFocusForGamepadOnly);
	DefineBitfieldUProperty(bShouldActivateWhenFocused);
	DefineBitfieldUProperty(bShowDirectionalArrowWhenFarOff);
	DefineBitfieldUProperty(bItemCanBeStolen);
	DefineBitfieldUProperty(bCanBeDepositedInStorageVault);
	DefineBitfieldUProperty(bItemHasDurability);
	DefineBitfieldUProperty(bOverridePickupMeshTransform);
public:
	bool ServerExecute(UFortItem* Item, AFortPlayerController* Instigator);

	UFortWorldItemDefinition* GetAmmoWorldItemDefinition_BP() const;
};

class UFortAmmoItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortAmmoItemDefinition);
};

class UFortResourceItemDefinition final : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortResourceItemDefinition);
};