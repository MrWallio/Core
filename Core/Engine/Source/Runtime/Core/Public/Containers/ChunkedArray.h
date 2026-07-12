#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/IndirectArray.h"

template<typename InElementType, uint32 TargetBytesPerChunk = 16384>
class TChunkedArray
{
public:
	using ElementType = InElementType;

	static constexpr int32 NumElementsPerChunk = TargetBytesPerChunk / sizeof(ElementType) > 1 ? TargetBytesPerChunk / sizeof(ElementType) : 1;

private:
	struct FChunk
	{
		ElementType Elements[NumElementsPerChunk];
	};

public:
	TIndirectArray<FChunk> Chunks;
	int32 NumElements;

public:
	TChunkedArray()
		: NumElements(0)
	{
	}

public:
	inline int32 Num() const { return NumElements; }

	inline bool IsValidIndex(int32 Index) const { return Index >= 0 && Index < NumElements; }

public:
	inline ElementType& operator[](int32 Index)
	{
		return Chunks[Index / NumElementsPerChunk].Elements[Index % NumElementsPerChunk];
	}

	inline const ElementType& operator[](int32 Index) const
	{
		return Chunks[Index / NumElementsPerChunk].Elements[Index % NumElementsPerChunk];
	}

public:
	int32 AddElement(const ElementType& Item)
	{
		if (NumElements + 1 > Chunks.Num() * NumElementsPerChunk)
		{
			FChunk* NewChunk = (FChunk*)FMemory::Malloc(sizeof(FChunk));
			std::memset((void*)NewChunk, 0, sizeof(FChunk));
			Chunks.Add(NewChunk);
		}

		const int32 Index = NumElements++;
		std::memcpy((void*)&(*this)[Index], (const void*)&Item, sizeof(ElementType));
		return Index;
	}

	void Empty()
	{
		Chunks.Empty();
		NumElements = 0;
	}
};

static_assert(sizeof(TChunkedArray<int32>) == 0x18, "TChunkedArray layout broke: chunk array + NumElements expected (0x18 on x64)");
