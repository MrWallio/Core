#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBaseUtility.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"

FString UObjectBaseUtility::GetPathName(const UObject* StopOuter) const
{
	FString Result = FString();
	GetPathName(StopOuter, Result);
	return Result;
}

void UObjectBaseUtility::GetPathName(const UObject* StopOuter, FString& ResultString) const
{
	void (*GetPathNameInternal)(const UObjectBaseUtility*, const UObject*, FString&) = decltype(GetPathNameInternal)(ImageBase + Finder::FindUObjectBaseUtility_GetPathName());
	GetPathNameInternal(this, StopOuter, ResultString);
}

UPackage* UObjectBaseUtility::GetOutermost() const
{
	UPackage* (*GetOutermostInternal)(const UObjectBaseUtility*) = decltype(GetOutermostInternal)(ImageBase + Finder::FindUObjectBaseUtility_GetOutermost());
	return GetOutermostInternal(this);
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