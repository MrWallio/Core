#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBaseUtility.h"

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

bool UObjectBaseUtility::MarkPackageDirty() const
{
	bool (*MarkPackageDirtyInternal)(const UObjectBaseUtility*) = decltype(MarkPackageDirtyInternal)(ImageBase + Finder::FindUObjectBaseUtility_MarkPackageDirty());
	return MarkPackageDirtyInternal(this);
}

UObject* UObjectBaseUtility::GetTypedOuter(UClass* Target) const
{
	UObject* (*GetTypedOuterInternal)(const UObjectBaseUtility*, UClass*) = decltype(GetTypedOuterInternal)(ImageBase + Finder::FindUObjectBaseUtility_GetTypedOuter());
	return GetTypedOuterInternal(this, Target);
}

bool UObjectBaseUtility::IsIn(const UObject* SomeOuter) const
{
	bool (*IsInInternal)(const UObjectBaseUtility*, const UObject*) = decltype(IsInInternal)(ImageBase + Finder::FindUObjectBaseUtility_IsIn());
	return IsInInternal(this, SomeOuter);
}

void* UObjectBaseUtility::GetInterfaceAddress(UClass* InterfaceClass)
{
	void* (*GetInterfaceAddressInternal)(UObjectBaseUtility*, UClass*) = decltype(GetInterfaceAddressInternal)(ImageBase + Finder::FindUObjectBaseUtility_GetInterfaceAddress());
	return GetInterfaceAddressInternal(this, InterfaceClass);
}

void* UObjectBaseUtility::GetNativeInterfaceAddress(UClass* InterfaceClass)
{
	void* (*GetNativeInterfaceAddressInternal)(UObjectBaseUtility*, UClass*) = decltype(GetNativeInterfaceAddressInternal)(ImageBase + Finder::FindUObjectBaseUtility_GetNativeInterfaceAddress());
	return GetNativeInterfaceAddressInternal(this, InterfaceClass);
}