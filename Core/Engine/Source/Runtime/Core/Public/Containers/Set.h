#pragma once
#include "pch.h"

#include "SparseArray.h"

template<typename SetType>
class SetElement
{
private:
	template<typename SetDataType>
	friend class TSet;

private:
	SetType Value;
	int32 HashNextId;
	int32 HashIndex;
};

template<typename SetElementType>
class TSet
{
private:
	static constexpr uint32 ElementAlign = alignof(SetElementType);
	static constexpr uint32 ElementSize = sizeof(SetElementType);

private:
	using SetDataType = SetElement<SetElementType>;
	using HashType = TInlineAllocator<1>::ForElementType<int32>;

private:
	TSparseArray<SetDataType> Elements;
	HashType Hash;
	int32 HashSize;

public:
	TSet()
		: HashSize(0)
	{
	}

	TSet(TSet&&) = default;
	TSet(const TSet&) = default;

public:
	TSet& operator=(TSet&&) = default;
	TSet& operator=(const TSet&) = default;

private:
	inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

public:
	inline int32 NumAllocated() const { return Elements.NumAllocated(); }

	inline int32 Num() const { return Elements.Num(); }
	inline int32 Max() const { return Elements.Max(); }

	inline bool IsValidIndex(int32 Index) const { return Elements.IsValidIndex(Index); }

	inline bool IsValid() const { return Elements.IsValid(); }

public:
	const FBitArray& GetAllocationFlags() const { return Elements.GetAllocationFlags(); }

public:
	inline       SetElementType& operator[] (int32 Index) { return Elements[Index].Value; }
	inline const SetElementType& operator[] (int32 Index) const { return Elements[Index].Value; }

	inline bool operator==(const TSet<SetElementType>& Other) const { return Elements == Other.Elements; }
	inline bool operator!=(const TSet<SetElementType>& Other) const { return Elements != Other.Elements; }

public:
	template<typename T> friend ContainerIterators::TSetIterator<T> begin(const TSet& Set);
	template<typename T> friend ContainerIterators::TSetIterator<T> end(const TSet& Set);
};

template<typename T> inline ContainerIterators::TSetIterator<T> begin(const TSet<T>& Set) { return ContainerIterators::TSetIterator<T>(Set, Set.GetAllocationFlags(), 0); }
template<typename T> inline ContainerIterators::TSetIterator<T> end(const TSet<T>& Set) { return ContainerIterators::TSetIterator<T>(Set, Set.GetAllocationFlags(), Set.NumAllocated()); }