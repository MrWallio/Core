#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition;
class AFortPawn;
class AFortPickup;
class AFortPickupEffect;
class IFortInventoryOwnerInterface;

struct FFortPickupLocationData
{
public:
	DefineUnrealStruct(FFortPickupLocationData);
};