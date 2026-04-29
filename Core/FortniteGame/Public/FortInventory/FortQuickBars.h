#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition;
class UFortItem;
class UFortWorldItem;

class AFortQuickBars : public AActor {
public:
	DefineUnrealClass(AFortQuickBars);
public:
	void ServerAddItemInternal(const FGuid& Item, EFortQuickBars InQuickBar, int32 Slot);

	static EFortQuickBars GetQuickBarForItem(UFortItemDefinition* ItemDefinition);
	static EFortQuickBars GetQuickBarForItem(UFortWorldItem* Item);
};