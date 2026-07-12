#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"
#include "Engine/Source/Runtime/Core/Public/Containers/ContainerAllocationPolicies.h"

template<typename ArrayElementType>
class TArray
{
protected:
	static constexpr uint64 ElementSize = sizeof(ArrayElementType);

public:
	ArrayElementType* Data;
	int32 ArrayNum;
	int32 ArrayMax;

public:
	TArray()
		: Data(nullptr), ArrayNum(0), ArrayMax(0)
	{
	}

	TArray(const TArray&) = default;

	TArray(TArray&&) = default;

public:
	TArray& operator=(TArray&&) = default;
	TArray& operator=(const TArray&) = default;

public:
	inline int32 GetSlack() const { return ArrayMax - ArrayNum; }

	static inline int32 GetTypeSize() { return (int32)ElementSize; }

	inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

	inline       ArrayElementType& GetUnsafe(int32 Index) { return Data[Index]; }
	inline const ArrayElementType& GetUnsafe(int32 Index) const { return Data[Index]; }

	inline ArrayElementType& GetWithSize(int32 Index, int32 Size = ElementSize)
	{
		return *(ArrayElementType*)((uint8*)Data + Index * Size);
	}
	inline const ArrayElementType& GetWithSize(int32 Index, int32 Size = ElementSize) const
	{
		return *(ArrayElementType*)((uint8*)Data + Index * Size);
	}

public:
	void CheckInvariants() const
	{
		checkSlow((ArrayNum >= 0) & (ArrayMax >= ArrayNum));
	}

	void DestructItems(ArrayElementType* Items, int32 Count)
	{
		if constexpr (!std::is_trivially_destructible_v<ArrayElementType>)
		{
			for (int32 i = 0; i < Count; ++i)
			{
				Items[i].~ArrayElementType();
			}
		}
	}

	void ResizeShrink(int32 Size = ElementSize)
	{
		if (ArrayNum != ArrayMax)
		{
			ArrayMax = ArrayNum;
			check(ArrayMax >= ArrayNum);
			ResizeTo(ArrayMax, Size);
		}
	}

	void ResizeTo(int32 NewMax, int32 Size = ElementSize)
	{
		if (NewMax != ArrayMax)
		{
			ArrayMax = NewMax;
			void* NewData = FMemory::Realloc(Data, ArrayMax * Size);
			if (NewData)
			{
				Data = (ArrayElementType*)NewData;
			}
		}
	}

	inline void GrowTo(int32 NewNum, int32 Size = ElementSize)
	{
		if (NewNum > ArrayMax)
		{
			ResizeTo(DefaultCalculateSlackGrow(NewNum, ArrayMax), Size);
		}
	}

	inline void Reserve(int32 Number, int32 Size = ElementSize)
	{
		if (Number > ArrayMax)
		{
			ResizeTo(Number, Size);
		}
	}

	inline void Free()
	{
		if (Data)
		{
			FMemory::Free(Data);
			Data = nullptr;
			ArrayNum = 0;
			ArrayMax = 0;
		}
	}

public:
	int32 AddUninitialized(int32 Count = 1, int32 Size = ElementSize)
	{
		if (Count <= 0)
			return ArrayNum;

		GrowTo(ArrayNum + Count, Size);

		const int32 OldNum = ArrayNum;
		ArrayNum += Count;
		return OldNum;
	}

	int32 AddZeroed(int32 Count = 1, int32 Size = ElementSize)
	{
		const int32 Index = AddUninitialized(Count, Size);
		if (Count > 0)
		{
			std::memset((uint8*)Data + ((size_t)Index * Size), 0, (size_t)Count * Size);
		}
		return Index;
	}

	int32 AddDefaulted(int32 Count = 1, int32 Size = ElementSize)
	{
		return AddZeroed(Count, Size);
	}

	int32 Add(const ArrayElementType& Item, int32 Size = ElementSize) {
		const int32 Index = AddUninitialized(1, Size);
		std::memcpy(PBYTE(Data) + ((size_t)Index * Size), (const PBYTE)&Item, Size);
		return Index;
	}

	int32 AddUnique(const ArrayElementType& Item, int32 Size = ElementSize)
	{
		int32 Index = Find(Item);
		if (Index != -1)
		{
			return Index;
		}
		return Add(Item, Size);
	}

	void Insert(const ArrayElementType& Item, int32 Index, int32 Size = ElementSize)
	{
		if (Index < 0 || Index > ArrayNum)
			return;

		AddUninitialized(1, Size);

		const int32 NumToMove = ArrayNum - Index - 1;
		if (NumToMove > 0)
		{
			std::memmove(
				(uint8*)Data + ((size_t)(Index + 1) * Size),
				(uint8*)Data + ((size_t)Index * Size),
				(size_t)NumToMove * Size);
		}

		std::memcpy((uint8*)Data + ((size_t)Index * Size), (const uint8*)&Item, Size);
	}

	int32 Push(const ArrayElementType& Item, int32 Size = ElementSize)
	{
		return Add(Item, Size);
	}

	ArrayElementType Pop()
	{
		ArrayElementType Result = (*this)[ArrayNum - 1];
		--ArrayNum;
		return Result;
	}

	inline       ArrayElementType& Top() { return (*this)[ArrayNum - 1]; }
	inline const ArrayElementType& Top() const { return (*this)[ArrayNum - 1]; }

	inline       ArrayElementType& Last(int32 IndexFromTheEnd = 0) { return (*this)[ArrayNum - IndexFromTheEnd - 1]; }
	inline const ArrayElementType& Last(int32 IndexFromTheEnd = 0) const { return (*this)[ArrayNum - IndexFromTheEnd - 1]; }

	void RemoveAtImpl(int32 Index, int32 Count, bool bAllowShrinking, int32 Size = ElementSize)
	{
		if (Count <= 0 || !IsValidIndex(Index))
			return;

		if (Index + Count > ArrayNum)
			Count = ArrayNum - Index;

		const int32 NumToMove = ArrayNum - Index - Count;
		if (NumToMove > 0)
		{
			std::memmove(
				(uint8*)Data + ((size_t)Index * Size),
				(uint8*)Data + ((size_t)(Index + Count) * Size),
				(size_t)NumToMove * Size);
		}

		ArrayNum -= Count;
	}

	void RemoveAt(int32 Index, int32 Size = ElementSize)
	{
		RemoveAtImpl(Index, 1, true, Size);
	}

	template <typename CountType>
	void RemoveAt(int32 Index, CountType Count, bool bAllowShrinking = true, int32 Size = ElementSize)
	{
		RemoveAtImpl(Index, Count, bAllowShrinking, Size);
	}

	void RemoveAtSwap(int32 Index, int32 Size = ElementSize)
	{
		if (!IsValidIndex(Index))
			return;

		if (Index != ArrayNum - 1)
		{
			std::memcpy(
				(uint8*)Data + ((size_t)Index * Size),
				(uint8*)Data + ((size_t)(ArrayNum - 1) * Size),
				Size);
		}

		--ArrayNum;
	}

	int32 Remove(const ArrayElementType& Item, int32 Size = ElementSize)
	{
		return RemoveAll([&Item](const ArrayElementType& Element) { return Element == Item; }, Size);
	}

	int32 RemoveSingle(const ArrayElementType& Item, int32 Size = ElementSize)
	{
		const int32 Index = Find(Item);
		if (Index == -1)
		{
			return 0;
		}

		RemoveAt(Index, Size);
		return 1;
	}

	template <typename PredicateType>
	int32 RemoveAll(PredicateType Predicate, int32 Size = ElementSize)
	{
		int32 NumRemoved = 0;
		for (int32 Index = ArrayNum - 1; Index >= 0; --Index)
		{
			if (Predicate((const ArrayElementType&)GetWithSize(Index, Size)))
			{
				RemoveAt(Index, Size);
				++NumRemoved;
			}
		}
		return NumRemoved;
	}

	void SetNumUninitialized(int32 NewNum, int32 Size = ElementSize)
	{
		if (NewNum < 0)
			return;

		if (NewNum > ArrayNum)
		{
			GrowTo(NewNum, Size);
		}

		ArrayNum = NewNum;
	}

	void SetNumZeroed(int32 NewNum, int32 Size = ElementSize)
	{
		const int32 OldNum = ArrayNum;
		SetNumUninitialized(NewNum, Size);

		if (NewNum > OldNum)
		{
			std::memset((uint8*)Data + ((size_t)OldNum * Size), 0, (size_t)(NewNum - OldNum) * Size);
		}
	}

	void SetNum(int32 NewNum, int32 Size = ElementSize)
	{
		SetNumZeroed(NewNum, Size);
	}

	void Append(const ArrayElementType* Items, int32 Count, int32 Size = ElementSize)
	{
		if (!Items || Count <= 0)
			return;

		const int32 Index = AddUninitialized(Count, Size);
		std::memcpy((uint8*)Data + ((size_t)Index * Size), (const uint8*)Items, (size_t)Count * Size);
	}

	void Append(const TArray<ArrayElementType>& Other, int32 Size = ElementSize)
	{
		Append(Other.GetData(), Other.Num(), Size);
	}

	TArray& operator+=(const TArray<ArrayElementType>& Other)
	{
		Append(Other);
		return *this;
	}

	void Init(const ArrayElementType& Item, int32 Number, int32 Size = ElementSize)
	{
		Empty(Number, Size);
		for (int32 i = 0; i < Number; ++i)
		{
			Add(Item, Size);
		}
	}

	void SwapMemory(int32 FirstIndex, int32 SecondIndex, int32 Size = ElementSize)
	{
		uint8* First = (uint8*)Data + ((size_t)FirstIndex * Size);
		uint8* Second = (uint8*)Data + ((size_t)SecondIndex * Size);

		for (int32 i = 0; i < Size; ++i)
		{
			const uint8 Temp = First[i];
			First[i] = Second[i];
			Second[i] = Temp;
		}
	}

	void Swap(int32 FirstIndex, int32 SecondIndex, int32 Size = ElementSize)
	{
		if (IsValidIndex(FirstIndex) && IsValidIndex(SecondIndex) && FirstIndex != SecondIndex)
		{
			SwapMemory(FirstIndex, SecondIndex, Size);
		}
	}

	void Reset(int32 NewSize = 0, int32 Size = ElementSize)
	{
		if constexpr (!std::is_trivially_destructible_v<ArrayElementType>)
		{
			for (int32 i = 0; i < ArrayNum; ++i)
			{
				Data[i].~ArrayElementType();
			}
		}

		ArrayNum = 0;

		if (NewSize > 0 && NewSize > ArrayMax)
		{
			Reserve(NewSize - ArrayNum, Size);
		}
	}

	void Empty(int32 Slack = 0, int32 Size = ElementSize)
	{
		DestructItems(GetData(), ArrayNum);

		ArrayNum = 0;

		if (ArrayMax != Slack)
		{
			ResizeTo(Slack, Size);
		}
	}

public:
	template <typename ComparisonType>
	bool Contains(const ComparisonType& Item) const
	{
		const ArrayElementType* DataPtr = GetData();
		const ArrayElementType* DataEnd = DataPtr + ArrayNum;

		for (; DataPtr != DataEnd; ++DataPtr)
		{
			if (*DataPtr == Item)
			{
				return true;
			}
		}
		return false;
	}

	template <typename PredicateType>
	bool ContainsByPredicate(PredicateType Predicate) const
	{
		return FindByPredicate(Predicate) != nullptr;
	}

	int32 Find(const ArrayElementType& Item) const
	{
		const ArrayElementType* DataPtr = GetData();
		for (int32 Index = 0; Index < ArrayNum; ++Index)
		{
			if (DataPtr[Index] == Item)
			{
				return Index;
			}
		}
		return -1;
	}

	bool Find(const ArrayElementType& Item, int32& OutIndex) const
	{
		OutIndex = Find(Item);
		return OutIndex != -1;
	}

	template <typename PredicateType>
	ArrayElementType* FindByPredicate(PredicateType Predicate)
	{
		ArrayElementType* DataPtr = GetData();
		for (int32 Index = 0; Index < ArrayNum; ++Index)
		{
			if (Predicate((const ArrayElementType&)DataPtr[Index]))
			{
				return &DataPtr[Index];
			}
		}
		return nullptr;
	}

	template <typename PredicateType>
	const ArrayElementType* FindByPredicate(PredicateType Predicate) const
	{
		return const_cast<TArray*>(this)->FindByPredicate(Predicate);
	}

	template <typename PredicateType>
	int32 IndexOfByPredicate(PredicateType Predicate) const
	{
		const ArrayElementType* DataPtr = GetData();
		for (int32 Index = 0; Index < ArrayNum; ++Index)
		{
			if (Predicate(DataPtr[Index]))
			{
				return Index;
			}
		}
		return -1;
	}

public:
	inline int32 Num() const { return ArrayNum; }
	inline int32 Max() const { return ArrayMax; }

	inline bool IsEmpty() const { return ArrayNum == 0; }

	inline bool IsValidIndex(int32 Index) const { return Data && Index >= 0 && Index < ArrayNum; }

	inline bool IsValid() const { return Data && ArrayNum > 0 && ArrayMax >= ArrayNum; }

	FORCEINLINE ArrayElementType* GetData()
	{
		return (ArrayElementType*)Data;
	}

	FORCEINLINE const ArrayElementType* GetData() const
	{
		return (const ArrayElementType*)Data;
	}

public:
	inline       ArrayElementType& operator[](int32 Index) { VerifyIndex(Index); return Data[Index]; }
	inline const ArrayElementType& operator[](int32 Index) const { VerifyIndex(Index); return Data[Index]; }

	inline bool operator==(const TArray<ArrayElementType>& Other) const { return Data == Other.Data; }
	inline bool operator!=(const TArray<ArrayElementType>& Other) const { return Data != Other.Data; }

	inline explicit operator bool() const { return IsValid(); };

public:
	ArrayElementType* begin()
	{
		return Data;
	}

	ArrayElementType* end()
	{
		return Data + ArrayNum;
	}

	const ArrayElementType* begin() const
	{
		return Data;
	}

	const ArrayElementType* end() const
	{
		return Data + ArrayNum;
	}
};

static_assert(sizeof(TArray<int32>) == 0x10, "TArray layout broke: UE 4.0-5.x expects Data/ArrayNum/ArrayMax = 0x10 on x64");
static_assert(offsetof(TArray<int32>, ArrayNum) == 0x8, "TArray::ArrayNum must sit at 0x8 to match the engine");
static_assert(offsetof(TArray<int32>, ArrayMax) == 0xC, "TArray::ArrayMax must sit at 0xC to match the engine");
