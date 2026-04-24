#pragma once
#include "pch.h"

#include "Set.h"

template <typename KeyType, typename ValueType>
class TPair
{
public:
	KeyType First;
	ValueType Second;

public:
	TPair(KeyType Key, ValueType Value)
		: First(Key), Second(Value)
	{
	}

public:
	inline       KeyType& Key() { return First; }
	inline const KeyType& Key() const { return First; }

	inline       ValueType& Value() { return Second; }
	inline const ValueType& Value() const { return Second; }
};

template<typename KeyElementType, typename ValueElementType>
class TMap
{
public:
	using ElementType = TPair<KeyElementType, ValueElementType>;

private:
	TSet<ElementType> Elements;

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
	inline       ElementType& operator[] (int32 Index) { return Elements[Index]; }
	inline const ElementType& operator[] (int32 Index) const { return Elements[Index]; }

	inline bool operator==(const TMap<KeyElementType, ValueElementType>& Other) const { return Elements == Other.Elements; }
	inline bool operator!=(const TMap<KeyElementType, ValueElementType>& Other) const { return Elements != Other.Elements; }

public:
	template<typename KeyType, typename ValueType> friend ContainerIterators::TMapIterator<KeyType, ValueType> begin(const TMap& Map);
	template<typename KeyType, typename ValueType> friend ContainerIterators::TMapIterator<KeyType, ValueType> end(const TMap& Map);
};

template<typename T0, typename T1> inline ContainerIterators::TMapIterator<T0, T1> begin(const TMap<T0, T1>& Map) { return ContainerIterators::TMapIterator<T0, T1>(Map, Map.GetAllocationFlags(), 0); }
template<typename T0, typename T1> inline ContainerIterators::TMapIterator<T0, T1> end(const TMap<T0, T1>& Map) { return ContainerIterators::TMapIterator<T0, T1>(Map, Map.GetAllocationFlags(), Map.NumAllocated()); }