#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"

template<typename KeyType, typename ValueType>
class TMapBuilder
{
private:
	TMap<KeyType, ValueType> Map;

public:
	TMapBuilder() = default;

	TMapBuilder& Add(const KeyType& Key, const ValueType& Value)
	{
		Map.Add(Key, Value);
		return *this;
	}

	TMap<KeyType, ValueType> Build() const
	{
		return Map;
	}

	operator TMap<KeyType, ValueType>() const
	{
		return Map;
	}
};
