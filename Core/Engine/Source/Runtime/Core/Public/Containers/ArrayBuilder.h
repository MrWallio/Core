#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/Array.h"

template<typename ElementType, typename AllocatorType = void>
class TArrayBuilder
{
private:
	TArray<ElementType> Array;

public:
	TArrayBuilder() = default;

	TArrayBuilder& Add(const ElementType& Item)
	{
		Array.Add(Item);
		return *this;
	}

	TArrayBuilder& Append(const TArray<ElementType>& OtherArray)
	{
		Array.Append(OtherArray);
		return *this;
	}

	TArray<ElementType> Build() const
	{
		return Array;
	}

	operator TArray<ElementType>() const
	{
		return Array;
	}
};
