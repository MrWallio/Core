#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

template<class TEnum>
class TEnumAsByte
{
private:
	uint8 Value;

public:
	TEnumAsByte()
		: Value(0)
	{
	}

	TEnumAsByte(TEnum InValue)
		: Value((uint8)InValue)
	{
	}

	explicit TEnumAsByte(int32 InValue)
		: Value((uint8)InValue)
	{
	}

public:
	operator TEnum() const { return (TEnum)Value; }

	TEnum GetValue() const { return (TEnum)Value; }
	uint8 GetIntValue() const { return Value; }

public:
	bool operator==(TEnum InValue) const { return Value == (uint8)InValue; }
	bool operator==(TEnumAsByte InValue) const { return Value == InValue.Value; }
	bool operator!=(TEnum InValue) const { return Value != (uint8)InValue; }
	bool operator!=(TEnumAsByte InValue) const { return Value != InValue.Value; }

public:
	friend uint32 GetTypeHash(const TEnumAsByte Enum)
	{
		return Enum.Value;
	}
};

namespace EnumAsByteChecks { enum ECheck { Zero }; }
static_assert(sizeof(TEnumAsByte<EnumAsByteChecks::ECheck>) == 0x1, "TEnumAsByte layout broke: must be exactly one byte");
