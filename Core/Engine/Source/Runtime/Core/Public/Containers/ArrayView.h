#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Array.h"

template<typename InElementType>
class TArrayView
{
public:
	using ElementType = InElementType;

private:
	ElementType* DataPtr;
	int32 ArrayNum;

public:
	TArrayView()
		: DataPtr(nullptr), ArrayNum(0)
	{
	}

	TArrayView(ElementType* InData, int32 InCount)
		: DataPtr(InData), ArrayNum(InCount)
	{
	}

	TArrayView(TArray<ElementType>& InArray)
		: DataPtr(InArray.GetData()), ArrayNum(InArray.Num())
	{
	}

	TArrayView(const TArray<ElementType>& InArray)
		: DataPtr(const_cast<ElementType*>(InArray.GetData())), ArrayNum(InArray.Num())
	{
	}

	template<int32 N>
	TArrayView(ElementType(&InArray)[N])
		: DataPtr(InArray), ArrayNum(N)
	{
	}

public:
	inline ElementType* GetData() const { return DataPtr; }

	inline int32 Num() const { return ArrayNum; }

	inline bool IsEmpty() const { return ArrayNum == 0; }

	inline bool IsValidIndex(int32 Index) const { return Index >= 0 && Index < ArrayNum; }

	inline size_t GetTypeSize() const { return sizeof(ElementType); }

public:
	inline ElementType& operator[](int32 Index) const { return DataPtr[Index]; }

	inline ElementType& Last(int32 IndexFromTheEnd = 0) const { return DataPtr[ArrayNum - IndexFromTheEnd - 1]; }

	TArrayView Slice(int32 Index, int32 InNum) const
	{
		return TArrayView(DataPtr + Index, InNum);
	}

	TArrayView Left(int32 Count) const
	{
		return TArrayView(DataPtr, Count < ArrayNum ? Count : ArrayNum);
	}

	TArrayView Right(int32 Count) const
	{
		const int32 Clamped = Count < ArrayNum ? Count : ArrayNum;
		return TArrayView(DataPtr + ArrayNum - Clamped, Clamped);
	}

public:
	int32 Find(const ElementType& Item) const
	{
		for (int32 Index = 0; Index < ArrayNum; ++Index)
		{
			if (DataPtr[Index] == Item)
			{
				return Index;
			}
		}
		return -1;
	}

	bool Contains(const ElementType& Item) const
	{
		return Find(Item) != -1;
	}

	template <typename PredicateType>
	ElementType* FindByPredicate(PredicateType Predicate) const
	{
		for (int32 Index = 0; Index < ArrayNum; ++Index)
		{
			if (Predicate((const ElementType&)DataPtr[Index]))
			{
				return &DataPtr[Index];
			}
		}
		return nullptr;
	}

public:
	inline ElementType* begin() const { return DataPtr; }
	inline ElementType* end() const { return DataPtr + ArrayNum; }
};

template<typename ElementType>
TArrayView<ElementType> MakeArrayView(ElementType* Pointer, int32 Size)
{
	return TArrayView<ElementType>(Pointer, Size);
}

template<typename ElementType>
TArrayView<ElementType> MakeArrayView(TArray<ElementType>& Array)
{
	return TArrayView<ElementType>(Array);
}

static_assert(sizeof(TArrayView<int32>) == 0x10, "TArrayView layout broke: pointer + count expected (0x10 on x64)");
