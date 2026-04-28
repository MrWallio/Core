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

void* UObjectBaseUtility::GetInterfaceAddress(UClass* InterfaceClass)
{
	void* (*GetInterfaceAddressInternal)(UObjectBaseUtility*, UClass*) = decltype(GetInterfaceAddressInternal)(ImageBase + Finder::FindUObjectBaseUtility_GetInterfaceAddress());
	return GetInterfaceAddressInternal(this, InterfaceClass);
}