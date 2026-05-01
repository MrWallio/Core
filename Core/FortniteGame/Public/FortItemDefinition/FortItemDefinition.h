#pragma once
#include "pch.h"

#include "McpItemDefinitionBase.h"

class UFortItemDefinition : public UMcpItemDefinitionBase {
public:
	DefineUnrealClass(UFortItemDefinition);

	DefineUProperty(FScalableFloat, MaxStackSize);
public:
	UFortItem* CreateTemporaryItemInstanceBP(int32 Count, int32 Level) const;

	int32 GetMaxStackSize() const;

	bool IsStackable() const;

	int32 GetClipSize();
};

class UFortAccountItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortAccountItemDefinition);
};