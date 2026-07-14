#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBaseUtility.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Package.h"

FString UObjectBaseUtility::GetPathName(const UObject* StopOuter) const
{
	FString Result = FString();
	GetPathName(StopOuter, Result);
	return Result;
}

void UObjectBaseUtility::GetPathName(const UObject* StopOuter, FString& ResultString) const
{
	if (this != (const UObjectBaseUtility*)StopOuter && this != nullptr)
	{
		UObject* ObjOuter = GetOuter();
		if (ObjOuter && ObjOuter != StopOuter)
		{
			ObjOuter->GetPathName(StopOuter, ResultString);

			if (ObjOuter->GetClass() != UPackage::StaticClass()
				&& ObjOuter->GetOuter()
				&& ObjOuter->GetOuter()->GetClass() == UPackage::StaticClass())
			{
				ResultString += FString(":");
			}
			else
			{
				ResultString += FString(".");
			}
		}

		ResultString += GetName();
	}
	else
	{
		ResultString += FString("None");
	}
}

UPackage* UObjectBaseUtility::GetOutermost() const
{
	UObject* Top = (UObject*)this;

	while (Top && Top->GetOuter())
	{
		Top = Top->GetOuter();
	}

	return (UPackage*)Top;
}

UObject* UObjectBaseUtility::GetTypedOuter(UClass* Target) const
{
	for (UObject* NextOuter = GetOuter(); NextOuter; NextOuter = NextOuter->GetOuter())
	{
		if (NextOuter->IsA(Target))
		{
			return NextOuter;
		}
	}

	return nullptr;
}

bool UObjectBaseUtility::IsIn(const UObject* SomeOuter) const
{
	for (UObject* NextOuter = GetOuter(); NextOuter; NextOuter = NextOuter->GetOuter())
	{
		if (NextOuter == SomeOuter)
		{
			return true;
		}
	}

	return false;
}

bool UObjectBaseUtility::IsInA(const UClass* SomeBaseClass) const
{
	for (UObject* NextOuter = GetOuter(); NextOuter; NextOuter = NextOuter->GetOuter())
	{
		if (NextOuter->IsA((UClass*)SomeBaseClass))
		{
			return true;
		}
	}

	return false;
}

bool UObjectBaseUtility::IsTemplate(EObjectFlags TemplateTypes) const
{
	for (const UObjectBaseUtility* TestOuter = this; TestOuter; TestOuter = (const UObjectBaseUtility*)TestOuter->GetOuter())
	{
		if (TestOuter->HasAnyFlags(TemplateTypes))
		{
			return true;
		}
	}

	return false;
}

void* UObjectBaseUtility::GetInterfaceAddress(UClass* InterfaceClass)
{
	void* (*GetInterfaceAddressInternal)(UObjectBaseUtility*, UClass*) = decltype(GetInterfaceAddressInternal)(ImageBase + Finder::FindUObjectBaseUtility_GetInterfaceAddress());
	return GetInterfaceAddressInternal(this, InterfaceClass);
}