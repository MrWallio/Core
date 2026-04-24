#pragma once
#include "pch.h"

#include "UObjectBase.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"

struct FGuid;
class UPackage;
class UClass;

class UObjectBaseUtility : public UObjectBase {
public:
	FString GetPathName(const UObject* StopOuter = NULL) const;

	void GetPathName(const UObject* StopOuter, FString& ResultString) const;

	FORCEINLINE FString GetName() const
	{
		return GetFName().ToString();
	}

	UPackage* GetOutermost() const;

	bool MarkPackageDirty() const;

	UObject* GetTypedOuter(UClass* Target) const;

	bool IsIn(const UObject* SomeOuter) const;

	void* GetInterfaceAddress(UClass* InterfaceClass);

	void* GetNativeInterfaceAddress(UClass* InterfaceClass);

	const void* GetNativeInterfaceAddress(UClass* InterfaceClass) const
	{
		return const_cast<UObjectBaseUtility*>(this)->GetNativeInterfaceAddress(InterfaceClass);
	}
};