#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "BitArray.h"

template <typename KeyType, typename ValueType>
class TMap;

template <typename SetElementType>
class TSet;

template <typename SparseArrayElementType>
class TSparseArray;

namespace ContainerIterators {
	template<typename ArrayType>
	class TArrayIterator;

	template<class ContainerType>
	class TContainerIterator;

	template<typename SparseArrayElementType>
	using TSparseArrayIterator = TContainerIterator<TSparseArray<SparseArrayElementType>>;

	template<typename SetElementType>
	using TSetIterator = TContainerIterator<TSet<SetElementType>>;

	template<typename KeyElementType, typename ValueElementType>
	using TMapIterator = TContainerIterator<TMap<KeyElementType, ValueElementType>>;

	template<class ContainerType>
	class TContainerIterator
	{
	private:
		ContainerType& IteratedContainer;
		FBitArray::FSetBitIterator BitIterator;

	public:
		TContainerIterator(const ContainerType& Container, const FBitArray& BitArray, int32 StartIndex = 0x0)
			: IteratedContainer(const_cast<ContainerType&>(Container)), BitIterator(BitArray, StartIndex)
		{
		}

	public:
		inline int32 GetIndex() { return BitIterator.GetIndex(); }

		inline int32 IsValid() { return IteratedContainer.IsValidIndex(GetIndex()); }

	public:
		inline TContainerIterator& operator++() { ++BitIterator; return *this; }
		inline TContainerIterator& operator--() { --BitIterator; return *this; }

		inline       auto& operator*() { return IteratedContainer[GetIndex()]; }
		inline const auto& operator*() const { return IteratedContainer[GetIndex()]; }

		inline       auto* operator->() { return &IteratedContainer[GetIndex()]; }
		inline const auto* operator->() const { return &IteratedContainer[GetIndex()]; }

		inline bool operator==(const TContainerIterator& Other) const { return &IteratedContainer == &Other.IteratedContainer && BitIterator == Other.BitIterator; }
		inline bool operator!=(const TContainerIterator& Other) const { return &IteratedContainer != &Other.IteratedContainer || BitIterator != Other.BitIterator; }
	};
}