#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

template<class TClass>
class TSubclassOf
{
public:
	TSubclassOf() = default;

	inline TSubclassOf(const UClass* Class)
		: Class(const_cast<UClass*>(Class))
	{
	}

	inline const UClass* Get()
	{
		return Class;
	}

	inline operator const UClass* () const
	{
		return Class;
	}

	template<typename Target, typename = std::enable_if<std::is_base_of_v<Target, TClass>, bool>::type>
	inline operator TSubclassOf<Target>() const
	{
		return Class;
	}

	inline const UClass* operator->()
	{
		return Class;
	}

	inline TSubclassOf& operator=(const UClass* Class)
	{
		Class = Class;

		return *this;
	}

	inline bool operator==(const TSubclassOf& Other) const
	{
		return Class == Other.Class;
	}

	inline bool operator!=(const TSubclassOf& Other) const
	{
		return Class != Other.Class;
	}

	inline bool operator==(UClass* Other) const
	{
		return Class == Other;
	}

	inline bool operator!=(UClass* Other) const
	{
		return Class != Other;
	}

public:
	UClass* Class;
};