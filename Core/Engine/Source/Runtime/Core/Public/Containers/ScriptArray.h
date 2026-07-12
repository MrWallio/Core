#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"
#include "Engine/Source/Runtime/Core/Public/Containers/ContainerAllocationPolicies.h"

class FScriptArray
{
public:
	void* Data;
	int32 ArrayNum;
	int32 ArrayMax;

public:
	FScriptArray()
		: Data(nullptr), ArrayNum(0), ArrayMax(0)
	{
	}

public:
	inline void* GetData() { return Data; }
	inline const void* GetData() const { return Data; }

	inline int32 Num() const { return ArrayNum; }
	inline int32 Max() const { return ArrayMax; }

	inline bool IsValidIndex(int32 Index) const { return Index >= 0 && Index < ArrayNum; }

	inline int32 GetSlack() const { return ArrayMax - ArrayNum; }

	inline size_t GetAllocatedSize(int32 NumBytesPerElement) const { return (size_t)ArrayMax * NumBytesPerElement; }

public:
	int32 AddUninitialized(int32 Count, int32 NumBytesPerElement)
	{
		if (Count <= 0)
			return ArrayNum;

		const int32 OldNum = ArrayNum;
		ArrayNum += Count;

		if (ArrayNum > ArrayMax)
		{
			ArrayMax = DefaultCalculateSlackGrow(ArrayNum, ArrayMax);
			Data = FMemory::Realloc(Data, (size_t)ArrayMax * NumBytesPerElement);
		}

		return OldNum;
	}

	int32 AddZeroed(int32 Count, int32 NumBytesPerElement)
	{
		const int32 Index = AddUninitialized(Count, NumBytesPerElement);
		std::memset((uint8*)Data + ((size_t)Index * NumBytesPerElement), 0, (size_t)Count * NumBytesPerElement);
		return Index;
	}

	void InsertUninitialized(int32 Index, int32 Count, int32 NumBytesPerElement)
	{
		if (Index < 0 || Index > ArrayNum || Count <= 0)
			return;

		AddUninitialized(Count, NumBytesPerElement);

		const int32 NumToMove = ArrayNum - Index - Count;
		if (NumToMove > 0)
		{
			std::memmove(
				(uint8*)Data + ((size_t)(Index + Count) * NumBytesPerElement),
				(uint8*)Data + ((size_t)Index * NumBytesPerElement),
				(size_t)NumToMove * NumBytesPerElement);
		}
	}

	void Remove(int32 Index, int32 Count, int32 NumBytesPerElement)
	{
		if (Count <= 0 || !IsValidIndex(Index))
			return;

		if (Index + Count > ArrayNum)
			Count = ArrayNum - Index;

		const int32 NumToMove = ArrayNum - Index - Count;
		if (NumToMove > 0)
		{
			std::memmove(
				(uint8*)Data + ((size_t)Index * NumBytesPerElement),
				(uint8*)Data + ((size_t)(Index + Count) * NumBytesPerElement),
				(size_t)NumToMove * NumBytesPerElement);
		}

		ArrayNum -= Count;
	}

	void Empty(int32 Slack, int32 NumBytesPerElement)
	{
		ArrayNum = 0;

		if (ArrayMax != Slack)
		{
			ArrayMax = Slack;

			if (ArrayMax == 0)
			{
				if (Data)
				{
					FMemory::Free(Data);
					Data = nullptr;
				}
			}
			else
			{
				Data = FMemory::Realloc(Data, (size_t)ArrayMax * NumBytesPerElement);
			}
		}
	}

	void SetNumUninitialized(int32 NewNum, int32 NumBytesPerElement)
	{
		if (NewNum < 0)
			return;

		if (NewNum > ArrayNum)
		{
			AddUninitialized(NewNum - ArrayNum, NumBytesPerElement);
		}
		else
		{
			ArrayNum = NewNum;
		}
	}

	void SwapMemory(int32 A, int32 B, int32 NumBytesPerElement)
	{
		uint8* First = (uint8*)Data + ((size_t)A * NumBytesPerElement);
		uint8* Second = (uint8*)Data + ((size_t)B * NumBytesPerElement);

		for (int32 i = 0; i < NumBytesPerElement; ++i)
		{
			const uint8 Temp = First[i];
			First[i] = Second[i];
			Second[i] = Temp;
		}
	}
};

static_assert(sizeof(FScriptArray) == 0x10, "FScriptArray layout broke: must match TArray (Data/ArrayNum/ArrayMax = 0x10 on x64)");
