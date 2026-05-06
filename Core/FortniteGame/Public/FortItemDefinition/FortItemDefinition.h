#pragma once
#include "pch.h"

#include "McpItemDefinitionBase.h"
#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition : public UMcpItemDefinitionBase {
public:
	DefineUnrealClass(UFortItemDefinition);

	DefineUProperty(FScalableFloat, MaxStackSize);
public:
	UFortItem* CreateTemporaryItemInstanceBP(int32 Count, int32 Level) const;

	int32 GetMaxStackSize() const;

	bool IsStackable() const;

	int32 GetClipSize();

	EFortQuickBars GetQuickBarForItem();
};

class UFortAccountItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortAccountItemDefinition);
};