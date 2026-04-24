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
	static void EmitBaseReferences(UClass* RootClass);
public:
	void LowLevelRename(FName NewName, UObject* NewOuter = NULL);

	void Register(const TCHAR* PackageName, const TCHAR* Name);

	void DeferredRegister(UClass* UClassStaticClass, const TCHAR* PackageName, const TCHAR* Name);
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

	bool IsDefaultObject() const
	{
		return (ObjectFlags & EObjectFlags::RF_ClassDefaultObject);
	}

	UProperty* FindPropertyByName(std::string InName) const;
};