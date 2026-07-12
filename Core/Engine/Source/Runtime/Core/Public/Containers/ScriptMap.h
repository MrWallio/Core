#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/ScriptSet.h"

class FScriptMap
{
public:
	FScriptSet Pairs;

public:
	inline int32 Num() const { return Pairs.Num(); }
	inline int32 GetMaxIndex() const { return Pairs.GetMaxIndex(); }

	inline bool IsValidIndex(int32 Index) const { return Pairs.IsValidIndex(Index); }

	inline void* GetData(int32 Index, int32 NumBytesPerPairSetElement)
	{
		return Pairs.GetData(Index, NumBytesPerPairSetElement);
	}

	inline const void* GetData(int32 Index, int32 NumBytesPerPairSetElement) const
	{
		return Pairs.GetData(Index, NumBytesPerPairSetElement);
	}
};

static_assert(sizeof(FScriptMap) == 0x50, "FScriptMap layout broke: must match TMap (0x50 on x64)");
