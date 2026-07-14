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
		if (!this) {
			return "None";
		}

		if (!IsValidLowLevelFast()) {
			return "None";
		}

		return GetFName().ToString();
	}

	FORCEINLINE void GetName(FString& ResultString) const
	{
		ResultString = GetName();
	}

	UPackage* GetOutermost() const;

	UObject* GetTypedOuter(UClass* Target) const;

	template<typename T>
	FORCEINLINE T* GetTypedOuter() const
	{
		return (T*)GetTypedOuter(T::StaticClass());
	}

	UObject* GetOutermostObject() const
	{
		const UObjectBaseUtility* Top = this;

		while (Top && Top->GetOuter())
		{
			Top = (const UObjectBaseUtility*)Top->GetOuter();
		}

		return (UObject*)Top;
	}

	bool IsIn(const UObject* SomeOuter) const;

	bool IsInA(const UClass* SomeBaseClass) const;

	bool IsTemplate(EObjectFlags TemplateTypes = (EObjectFlags)(RF_ArchetypeObject | RF_ClassDefaultObject)) const;

	void* GetInterfaceAddress(UClass* InterfaceClass);
};