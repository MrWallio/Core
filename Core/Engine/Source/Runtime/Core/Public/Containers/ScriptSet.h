#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/ScriptArray.h"
#include "Engine/Source/Runtime/Core/Public/Containers/BitArray.h"

class FScriptSparseArray
{
public:
	FScriptArray Data;
	TBitArray AllocationFlags;
	int32 FirstFreeIndex;
	int32 NumFreeIndices;

public:
	FScriptSparseArray()
		: FirstFreeIndex(-1), NumFreeIndices(0)
	{
	}

public:
	inline int32 Num() const { return Data.Num() - NumFreeIndices; }
	inline int32 GetMaxIndex() const { return Data.Num(); }

	inline bool IsValidIndex(int32 Index) const { return Data.IsValidIndex(Index) && AllocationFlags[Index]; }

	inline void* GetData(int32 Index, int32 NumBytesPerElement)
	{
		return (uint8*)Data.GetData() + ((size_t)Index * NumBytesPerElement);
	}

	inline const void* GetData(int32 Index, int32 NumBytesPerElement) const
	{
		return (const uint8*)Data.GetData() + ((size_t)Index * NumBytesPerElement);
	}
};

class FScriptSet
{
public:
	FScriptSparseArray Elements;
	TInlineAllocator<1>::ForElementType<int32_t> Hash;
	int32 HashSize;

public:
	FScriptSet()
		: HashSize(0)
	{
	}

public:
	inline int32 Num() const { return Elements.Num(); }
	inline int32 GetMaxIndex() const { return Elements.GetMaxIndex(); }

	inline bool IsValidIndex(int32 Index) const { return Elements.IsValidIndex(Index); }

	inline void* GetData(int32 Index, int32 NumBytesPerSetElement)
	{
		return Elements.GetData(Index, NumBytesPerSetElement);
	}

	inline const void* GetData(int32 Index, int32 NumBytesPerSetElement) const
	{
		return Elements.GetData(Index, NumBytesPerSetElement);
	}
};

static_assert(sizeof(FScriptSparseArray) == 0x38, "FScriptSparseArray layout broke: must match TSparseArray (0x38 on x64)");
static_assert(sizeof(FScriptSet) == 0x50, "FScriptSet layout broke: must match TSet (0x50 on x64)");
