#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Array.h"

template<typename InElementType>
class TIndirectArray
{
public:
	using ElementType = InElementType;

public:
	TArray<void*> Array;

public:
	TIndirectArray() = default;

public:
	inline int32 Num() const { return Array.Num(); }
	inline int32 Max() const { return Array.Max(); }

	inline bool IsValidIndex(int32 Index) const { return Array.IsValidIndex(Index); }

	inline ElementType** GetData() { return (ElementType**)Array.GetData(); }
	inline const ElementType* const* GetData() const { return (const ElementType* const*)Array.GetData(); }

public:
	inline ElementType& operator[](int32 Index) { return *(ElementType*)Array[Index]; }
	inline const ElementType& operator[](int32 Index) const { return *(const ElementType*)Array[Index]; }

	inline ElementType& Last(int32 IndexFromTheEnd = 0) { return *(ElementType*)Array.Last(IndexFromTheEnd); }

public:
	int32 Add(ElementType* Item)
	{
		return Array.Add((void*)Item);
	}

	void Insert(ElementType* Item, int32 Index)
	{
		Array.Insert((void*)Item, Index);
	}

	void RemoveAt(int32 Index, bool bFreeElement = true)
	{
		if (!Array.IsValidIndex(Index))
			return;

		if (bFreeElement && Array[Index])
		{
			FMemory::Free(Array[Index]);
		}

		Array.RemoveAt(Index);
	}

	void RemoveAtSwap(int32 Index, bool bFreeElement = true)
	{
		if (!Array.IsValidIndex(Index))
			return;

		if (bFreeElement && Array[Index])
		{
			FMemory::Free(Array[Index]);
		}

		Array.RemoveAtSwap(Index);
	}

	void Empty(int32 Slack = 0, bool bFreeElements = true)
	{
		if (bFreeElements)
		{
			for (int32 Index = 0; Index < Array.Num(); ++Index)
			{
				if (Array[Index])
				{
					FMemory::Free(Array[Index]);
				}
			}
		}

		Array.Empty(Slack);
	}

	inline void Reserve(int32 Number) { Array.Reserve(Number); }

public:
	class TIterator
	{
	private:
		void** Ptr;

	public:
		TIterator(void** InPtr) : Ptr(InPtr) {}

		inline TIterator& operator++() { ++Ptr; return *this; }

		inline ElementType& operator*() const { return *(ElementType*)*Ptr; }
		inline ElementType* operator->() const { return (ElementType*)*Ptr; }

		inline bool operator==(const TIterator& Other) const { return Ptr == Other.Ptr; }
		inline bool operator!=(const TIterator& Other) const { return Ptr != Other.Ptr; }
	};

	inline TIterator begin() { return TIterator(Array.GetData()); }
	inline TIterator end() { return TIterator(Array.GetData() + Array.Num()); }
	inline TIterator begin() const { return TIterator(const_cast<void**>(Array.GetData())); }
	inline TIterator end() const { return TIterator(const_cast<void**>(Array.GetData()) + Array.Num()); }
};

static_assert(sizeof(TIndirectArray<int32>) == 0x10, "TIndirectArray layout broke: must be a single TArray of pointers (0x10 on x64)");
