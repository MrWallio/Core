#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Array.h"
#include "Engine/Source/Runtime/Core/Public/Templates/Tuple.h"

template<typename KeyElementType, typename ValueElementType>
class TSortedMap
{
public:
	using ElementType = TPair<KeyElementType, ValueElementType>;

public:
	TArray<ElementType> Pairs;

private:
	int32 FindInsertIndex(const KeyElementType& Key, bool& bOutFound) const
	{
		int32 Lo = 0;
		int32 Hi = Pairs.Num();

		while (Lo < Hi)
		{
			const int32 Mid = Lo + (Hi - Lo) / 2;
			if (Pairs[Mid].Key() < Key)
			{
				Lo = Mid + 1;
			}
			else
			{
				Hi = Mid;
			}
		}

		bOutFound = Lo < Pairs.Num() && !(Key < Pairs[Lo].Key()) && !(Pairs[Lo].Key() < Key);
		return Lo;
	}

public:
	inline int32 Num() const { return Pairs.Num(); }

	inline bool IsEmpty() const { return Pairs.Num() == 0; }

public:
	ValueElementType* Find(const KeyElementType& Key)
	{
		bool bFound = false;
		const int32 Index = FindInsertIndex(Key, bFound);
		return bFound ? &Pairs[Index].Value() : nullptr;
	}

	const ValueElementType* Find(const KeyElementType& Key) const
	{
		return const_cast<TSortedMap*>(this)->Find(Key);
	}

	ValueElementType FindRef(const KeyElementType& Key) const
	{
		const ValueElementType* Found = Find(Key);
		return Found ? *Found : ValueElementType{};
	}

	ValueElementType& FindChecked(const KeyElementType& Key)
	{
		ValueElementType* Found = Find(Key);
		if (!Found)
		{
			throw std::out_of_range("TSortedMap::FindChecked: key not found!");
		}
		return *Found;
	}

	bool Contains(const KeyElementType& Key) const
	{
		return Find(Key) != nullptr;
	}

	ValueElementType& Add(const KeyElementType& Key, const ValueElementType& Value)
	{
		bool bFound = false;
		const int32 Index = FindInsertIndex(Key, bFound);

		if (bFound)
		{
			std::memcpy((void*)&Pairs[Index].Value(), (const void*)&Value, sizeof(ValueElementType));
			return Pairs[Index].Value();
		}

		Pairs.Insert(ElementType(Key, Value), Index);
		return Pairs[Index].Value();
	}

	ValueElementType& FindOrAdd(const KeyElementType& Key)
	{
		if (ValueElementType* Existing = Find(Key))
		{
			return *Existing;
		}

		return Add(Key, ValueElementType{});
	}

	bool Remove(const KeyElementType& Key)
	{
		bool bFound = false;
		const int32 Index = FindInsertIndex(Key, bFound);

		if (bFound)
		{
			Pairs.RemoveAt(Index);
			return true;
		}

		return false;
	}

	void GenerateKeyArray(TArray<KeyElementType>& OutArray) const
	{
		OutArray.Empty(Num());
		for (int32 Index = 0; Index < Pairs.Num(); ++Index)
		{
			OutArray.Add(Pairs[Index].Key());
		}
	}

	void GenerateValueArray(TArray<ValueElementType>& OutArray) const
	{
		OutArray.Empty(Num());
		for (int32 Index = 0; Index < Pairs.Num(); ++Index)
		{
			OutArray.Add(Pairs[Index].Value());
		}
	}

	inline void Empty(int32 Slack = 0) { Pairs.Empty(Slack); }
	inline void Reset() { Pairs.Reset(); }

public:
	inline ElementType* begin() { return Pairs.begin(); }
	inline ElementType* end() { return Pairs.end(); }
	inline const ElementType* begin() const { return Pairs.begin(); }
	inline const ElementType* end() const { return Pairs.end(); }
};

static_assert(sizeof(TSortedMap<int32, int32>) == 0x10, "TSortedMap layout broke: must be a single TArray of pairs (0x10 on x64)");
