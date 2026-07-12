#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

template <typename KeyType, typename ValueType>
class TPair
{
public:
	KeyType First;
	ValueType Second;

public:
	TPair()
		: First(), Second()
	{
	}

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

static_assert(sizeof(TPair<int32, int64>) == 0x10, "TPair layout broke: Key then Value with natural alignment expected");
