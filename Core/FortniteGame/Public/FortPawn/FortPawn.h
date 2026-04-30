#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Character.h"

#include "FortniteGame/Public/FortEnums.h"

class AFortWeapon;
class UFortWeaponItemDefinition;
class UFortItemDefinition;
class AFortPickup;
struct FFortPickupRequestInfo;
class UCustomCharacterPart;

class AFortPawn : public ACharacter {
public:
	DefineUnrealClass(AFortPawn);

	DefineUProperty(AFortWeapon*, CurrentWeapon);

	DefineBitfieldUProperty(bMovingEmote);
	DefineBitfieldUProperty(bMovingEmoteForwardOnly);
	DefineBitfieldUProperty(bMovingEmoteFollowingOnly);
	DefineBitfieldUProperty(bMovingEmoteSkipLandingFX);

	DefineUProperty(int32, EmoteCount);
	DefineUProperty(float, LastEmoteTime);
	DefineUProperty(float, LastEmoteEndTime);

	DefineUProperty(UFortItemDefinition*, LastEmoteItemDef);
	DefineUProperty(UFortItemDefinition*, LastReplicatedEmoteExecuted);
	DefineUProperty(bool, bFireBlockedByEmoteCooldown);

	DefineUProperty(float, EmoteToFireCooldownTime);
	DefineUProperty(float, EmoteWalkSpeed);

	DefineUProperty(TArray<AFortPickup*>, IncomingPickups);
public:
	AFortWeapon* EquipWeaponDefinition(const UFortWeaponItemDefinition* WeaponData, const FGuid& ItemEntryGuid);

	void OnRep_CurrentWeapon(AFortWeapon* OldWeapon);
};