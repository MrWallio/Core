#pragma once
#include "pch.h"

#include "BitArray.h"
#include "Array.h"
#include "ContainerIterator.h"

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

private:
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

	inline bool IsValidIndex(int32 Index) const { return Data.IsValidIndex(Index) && AllocationFlags[Index]; }

	inline bool IsValid() const { return Data.IsValid() && AllocationFlags.IsValid(); }

public:
	const FBitArray& GetAllocationFlags() const { return AllocationFlags; }

public:
	inline       SparseArrayElementType& operator[](int32 Index) { VerifyIndex(Index); return *reinterpret_cast<SparseArrayElementType*>(&Data.GetUnsafe(Index).ElementData); }
	inline const SparseArrayElementType& operator[](int32 Index) const { VerifyIndex(Index); return *reinterpret_cast<SparseArrayElementType*>(&Data.GetUnsafe(Index).ElementData); }

	inline bool operator==(const TSparseArray<SparseArrayElementType>& Other) const { return Data == Other.Data; }
	inline bool operator!=(const TSparseArray<SparseArrayElementType>& Other) const { return Data != Other.Data; }

public:
	template<typename T> friend ContainerIterators::TSparseArrayIterator<T> begin(const TSparseArray& Array);
	template<typename T> friend ContainerIterators::TSparseArrayIterator<T> end(const TSparseArray& Array);
};

template<typename T> inline ContainerIterators::TSparseArrayIterator<T> begin(const TSparseArray<T>& Array) { return ContainerIterators::TSparseArrayIterator<T>(Array, Array.GetAllocationFlags(), 0); }
template<typename T> inline ContainerIterators::TSparseArrayIterator<T> end(const TSparseArray<T>& Array) { return ContainerIterators::TSparseArrayIterator<T>(Array, Array.GetAllocationFlags(), Array.NumAllocated()); }