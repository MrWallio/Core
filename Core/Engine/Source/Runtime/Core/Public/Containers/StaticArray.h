#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

template<typename InElementType, uint32 NumElements>
class TStaticArray
{
public:
	using ElementType = InElementType;

public:
	ElementType Elements[NumElements];

public:
	TStaticArray()
		: Elements{}
	{
	}

public:
	static inline int32 Num() { return (int32)NumElements; }

	inline bool IsValidIndex(int32 Index) const { return Index >= 0 && Index < (int32)NumElements; }

	inline ElementType* GetData() { return Elements; }
	inline const ElementType* GetData() const { return Elements; }

public:
	inline ElementType& operator[](int32 Index) { return Elements[Index]; }
	inline const ElementType& operator[](int32 Index) const { return Elements[Index]; }

	bool operator==(const TStaticArray& Other) const
	{
		for (uint32 Index = 0; Index < NumElements; ++Index)
		{
			if (!(Elements[Index] == Other.Elements[Index]))
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const TStaticArray& Other) const { return !(*this == Other); }

public:
	inline ElementType* begin() { return Elements; }
	inline ElementType* end() { return Elements + NumElements; }
	inline const ElementType* begin() const { return Elements; }
	inline const ElementType* end() const { return Elements + NumElements; }
};

static_assert(sizeof(TStaticArray<int32, 4>) == 0x10, "TStaticArray layout broke: must be exactly the inline elements");
