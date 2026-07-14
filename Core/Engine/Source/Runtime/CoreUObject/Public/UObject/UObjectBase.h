#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "ObjectMacros.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"

class UClass;
class UObject;
class UProperty;

class UObjectBase {
public:
	bool IsValidLowLevel() const;

	bool IsValidLowLevelFast(bool bRecursive = true) const;

	FORCEINLINE uint32 GetUniqueID() const
	{
		return (uint32)InternalIndex;
	}
	FORCEINLINE UClass* GetClass() const
	{
		return ClassPrivate;
	}
	FORCEINLINE UObject* GetOuter() const
	{
		return (UObject*)OuterPrivate;
	}
	FORCEINLINE FName GetFName() const
	{
		return NamePrivate;
	}
	FORCEINLINE int32 GetInternalIndex() const
	{
		return InternalIndex;
	}

public:
	FORCEINLINE EObjectFlags GetFlags() const
	{
		return ObjectFlags;
	}
	FORCEINLINE void SetFlags(EObjectFlags NewFlags)
	{
		ObjectFlags |= NewFlags;
	}
	FORCEINLINE void ClearFlags(EObjectFlags FlagsToClear)
	{
		ObjectFlags &= ~FlagsToClear;
	}
	FORCEINLINE bool HasAnyFlags(EObjectFlags FlagsToCheck) const
	{
		return (ObjectFlags & FlagsToCheck) != 0;
	}
	FORCEINLINE bool HasAllFlags(EObjectFlags FlagsToCheck) const
	{
		return (ObjectFlags & FlagsToCheck) == FlagsToCheck;
	}
public:
	void** VTable;

public:
	/** Flags used to track and report various object states. This needs to be 8 byte aligned on 32-bit
		platforms to reduce memory waste */
	EObjectFlags					ObjectFlags;

	/** Index into GObjectArray...very private. */
	int32							InternalIndex;

	/** Class the object belongs to. */
	UClass* ClassPrivate;

	/** Name of this object */
	FName							NamePrivate;

	/** Object this object resides in. */
	UObjectBase* OuterPrivate;
public:
	// Custom
	std::string GetFullName() const;

	bool IsA(EClassCastFlags TypeFlags) const;

	bool IsA(UClass* TypeClass) const;

	template<typename T>
	inline T* Cast()
	{
		return IsA(T::StaticClass()) ? (T*)this : nullptr;
	}

	template<typename T>
	inline T* Cast() const
	{
		return IsA(T::StaticClass()) ? (T*)this : nullptr;
	}

	template<typename T>
	inline bool IsA() const
	{
		return IsA(T::StaticClass());
	}

	bool IsDefaultObject() const
	{
		return (ObjectFlags & EObjectFlags::RF_ClassDefaultObject);
	}

	UProperty* FindPropertyByName(FName InName) const;
};

static_assert(offsetof(UObjectBase, ObjectFlags) == 0x8, "UObjectBase::ObjectFlags must sit at 0x8 to match the engine");
static_assert(offsetof(UObjectBase, InternalIndex) == 0xC, "UObjectBase::InternalIndex must sit at 0xC to match the engine");
static_assert(offsetof(UObjectBase, ClassPrivate) == 0x10, "UObjectBase::ClassPrivate must sit at 0x10 to match the engine");
static_assert(offsetof(UObjectBase, NamePrivate) == 0x18, "UObjectBase::NamePrivate must sit at 0x18 to match the engine");
static_assert(offsetof(UObjectBase, OuterPrivate) == 0x20, "UObjectBase::OuterPrivate must sit at 0x20 to match the engine");