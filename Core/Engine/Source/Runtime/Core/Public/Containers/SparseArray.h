#pragma once
#include "pch.h"

#include "BitArray.h"
#include "Array.h"

template<typename SparseArrayType>
union TSparseArrayElementOrFreeListLink
{
	SparseArrayType ElementData;

	struct
	{
		int32 PrevFreeIndex;
		int32 NextFreeIndex;
	};
};

template<typename SparseArrayElementType>
class TSparseArray
{
private:
	static constexpr uint32 ElementAlign = alignof(SparseArrayElementType);
	static constexpr uint32 ElementSize = sizeof(SparseArrayElementType);

private:
	using FElementOrFreeListLink = TSparseArrayElementOrFreeListLink<TAlignedBytes<ElementSize, ElementAlign>>;

public:
	TArray<FElementOrFreeListLink> Data;
	FBitArray AllocationFlags;
	int32 FirstFreeIndex;
	int32 NumFreeIndices;

public:
	TSparseArray()
		: FirstFreeIndex(-1), NumFreeIndices(0)
	{
	}

	TSparseArray(TSparseArray&&) = default;
	TSparseArray(const TSparseArray&) = default;

public:
	TSparseArray& operator=(TSparseArray&&) = default;
	TSparseArray& operator=(const TSparseArray&) = default;

private:
	inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

public:
	inline int32 NumAllocated() const { return Data.Num(); }

	inline int32 Num() const { return NumAllocated() - NumFreeIndices; }
	inline int32 Max() const { return Data.Max(); }

	inline int32 GetMaxIndex() const { return Data.Num(); }

	inline bool IsValidIndex(int32 Index) const { return Data.IsValidIndex(Index) && AllocationFlags[Index]; }

	inline bool IsValid() const { return Data.IsValid() && AllocationFlags.IsValid(); }

	FORCEINLINE bool IsAllocated(int32 Index) const
	{
		if (Index < 0 || Index >= NumAllocated())
			return false;

		return AllocationFlags[Index];
	}
public:
	const FBitArray& GetAllocationFlags() const { return AllocationFlags; }
public:
	int32 AddUninitializedIndex()
	{
		int32 Index;

		if (NumFreeIndices)
		{
			Index = FirstFreeIndex;
			FirstFreeIndex = Data.GetUnsafe(Index).NextFreeIndex;
			--NumFreeIndices;

			if (NumFreeIndices)
			{
				Data.GetUnsafe(FirstFreeIndex).PrevFreeIndex = -1;
			}

			AllocationFlags.Set(Index, true);
		}
		else
		{
			Index = Data.AddUninitialized();
			AllocationFlags.Add(true);
		}

		return Index;
	}

	int32 Add(const SparseArrayElementType& Element)
	{
		const int32 Index = AddUninitializedIndex();
		std::memcpy((void*)&Data.GetUnsafe(Index).ElementData, (const void*)&Element, sizeof(SparseArrayElementType));
		return Index;
	}

	inline void RemoveAt(int32 Index, int32 Count = 1)
	{
		for (; Count; --Count)
		{
			if (NumFreeIndices) Data[FirstFreeIndex].PrevFreeIndex = Index;
			auto& IndexData = Data[Index];
			IndexData.PrevFreeIndex = -1;
			IndexData.NextFreeIndex = NumFreeIndices > 0 ? FirstFreeIndex : -1;
			FirstFreeIndex = Index;
			++NumFreeIndices;
			AllocationFlags.Set(Index, false);

			++Index;
		}
	}

	void Reset()
	{
		Data.Reset();
		AllocationFlags.Reset();
		FirstFreeIndex = -1;
		NumFreeIndices = 0;
	}

public:
	inline       SparseArrayElementType& operator[](int32 Index) { VerifyIndex(Index); return *reinterpret_cast<SparseArrayElementType*>(&Data.GetUnsafe(Index).ElementData); }
	inline const SparseArrayElementType& operator[](int32 Index) const { VerifyIndex(Index); return *reinterpret_cast<const SparseArrayElementType*>(&Data.GetUnsafe(Index).ElementData); }

	inline bool operator==(const TSparseArray<SparseArrayElementType>& Other) const { return Data == Other.Data; }
	inline bool operator!=(const TSparseArray<SparseArrayElementType>& Other) const { return Data != Other.Data; }

public:
	class TIterator
	{
	private:
		TSparseArray& Array;
		FBitArray::FSetBitIterator BitIt;

	public:
		TIterator(const TSparseArray& InArray, int32 StartIndex = 0)
			: Array(const_cast<TSparseArray&>(InArray))
			, BitIt(InArray.GetAllocationFlags(), StartIndex)
		{
		}

	public:
		inline int32 GetIndex() { return BitIt.GetIndex(); }

		inline TIterator& operator++() { ++BitIt; return *this; }

		inline explicit operator bool() const { return (bool)BitIt; }

		inline SparseArrayElementType& operator*() { return Array[GetIndex()]; }
		inline SparseArrayElementType* operator->() { return &Array[GetIndex()]; }

		inline bool operator==(const TIterator& Other) const { return BitIt == Other.BitIt; }
		inline bool operator!=(const TIterator& Other) const { return BitIt != Other.BitIt; }
	};

	inline TIterator begin() { return TIterator(*this, 0); }
	inline TIterator end() { return TIterator(*this, NumAllocated()); }
	inline TIterator begin() const { return TIterator(*this, 0); }
	inline TIterator end() const { return TIterator(*this, NumAllocated()); }
};

static_assert(sizeof(TSparseArray<int32>) == 0x38, "TSparseArray layout broke: UE 4.0-5.x expects 0x38 on x64");
static_assert(offsetof(TSparseArray<int32>, AllocationFlags) == 0x10, "TSparseArray::AllocationFlags must sit at 0x10 to match the engine");
static_assert(offsetof(TSparseArray<int32>, FirstFreeIndex) == 0x30, "TSparseArray::FirstFreeIndex must sit at 0x30 to match the engine");
static_assert(offsetof(TSparseArray<int32>, NumFreeIndices) == 0x34, "TSparseArray::NumFreeIndices must sit at 0x34 to match the engine");
