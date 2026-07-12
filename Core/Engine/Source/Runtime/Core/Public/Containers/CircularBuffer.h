#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Array.h"
#include "Engine/Source/Runtime/Core/Public/Containers/ContainerAllocationPolicies.h"

template<typename ElementType>
class TCircularBuffer
{
public:
	TArray<ElementType> Elements;
	uint32 IndexMask;

public:
	TCircularBuffer()
		: IndexMask(0)
	{
	}

	explicit TCircularBuffer(uint32 Capacity)
		: IndexMask(0)
	{
		Elements.SetNumZeroed((int32)RoundUpToPowerOfTwo(Capacity));
		IndexMask = (uint32)Elements.Num() - 1;
	}

	TCircularBuffer(uint32 Capacity, const ElementType& InitialValue)
		: IndexMask(0)
	{
		Elements.Init(InitialValue, (int32)RoundUpToPowerOfTwo(Capacity));
		IndexMask = (uint32)Elements.Num() - 1;
	}

public:
	inline uint32 Capacity() const { return (uint32)Elements.Num(); }

	inline uint32 GetNextIndex(uint32 CurrentIndex) const { return (CurrentIndex + 1) & IndexMask; }
	inline uint32 GetPreviousIndex(uint32 CurrentIndex) const { return (CurrentIndex - 1) & IndexMask; }

public:
	inline ElementType& operator[](uint32 Index) { return Elements.GetData()[Index & IndexMask]; }
	inline const ElementType& operator[](uint32 Index) const { return Elements.GetData()[Index & IndexMask]; }
};

static_assert(sizeof(TCircularBuffer<int32>) == 0x18, "TCircularBuffer layout broke: TArray + IndexMask expected (0x18 on x64)");
