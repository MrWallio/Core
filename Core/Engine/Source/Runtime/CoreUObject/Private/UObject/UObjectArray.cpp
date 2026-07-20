#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"

#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"

UObject* FUObjectArray::FindObject(const std::string& FullName, bool bStrictCheck, EClassCastFlags Flags)
{
	std::string LowerFull = Utils::StringToLower(FullName);

	if (bStrictCheck)
	{
		size_t Sep = FullName.find_last_of('.');
		FName ShortFName((Sep == std::string::npos ? FullName : FullName.substr(Sep + 1)).c_str());

		for (int i = 0; i < Num(); ++i)
		{
			FUObjectItem* Item = IndexToObject(i);
			if (!Item)
				continue;

			UObject* Object = (UObject*)Item->Object;
			if (!Object)
				continue;

			if (!Object->GetFName().IsEqual(ShortFName, ENameCase::IgnoreCase, false))
				continue;

			if (Utils::StringToLower(Object->GetFullName()) == LowerFull)
				return Object;
		}

		return nullptr;
	}

	for (int i = 0; i < Num(); ++i)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;
		if (!Object)
			continue;

		std::string objectFullName = Utils::StringToLower(Object->GetFullName());
		if (objectFullName.contains(LowerFull) && (Flags == EClassCastFlags::RF_NoFlags || Object->IsA(Flags)))
		{
			return Object;
		}
	}

	return nullptr;
}

UObject* FUObjectArray::FindObjectWithClass(const std::string& FullName, bool bStrictCheck, UClass* Class)
{
	std::string LowerFull = Utils::StringToLower(FullName);

	if (bStrictCheck)
	{
		size_t Sep = FullName.find_last_of('.');
		FName ShortFName((Sep == std::string::npos ? FullName : FullName.substr(Sep + 1)).c_str());

		for (int i = 0; i < Num(); ++i)
		{
			FUObjectItem* Item = IndexToObject(i);
			if (!Item)
				continue;

			UObject* Object = (UObject*)Item->Object;
			if (!Object)
				continue;

			if (!Object->GetFName().IsEqual(ShortFName, ENameCase::IgnoreCase, false))
				continue;

			if (Utils::StringToLower(Object->GetFullName()) == LowerFull)
				return Object;
		}

		return nullptr;
	}

	for (int i = 0; i < Num(); ++i)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;
		if (!Object)
			continue;

		std::string objectFullName = Utils::StringToLower(Object->GetFullName());
		if (objectFullName.contains(LowerFull) && (!Class || Object->IsA(Class)))
		{
			return Object;
		}
	}

	return nullptr;
}

UObject* FUObjectArray::FindObjectFast(FName Name)
{
	for (int i = 0; i < Num(); ++i)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (!Object)
			continue;

		if (Object->GetFName() == Name)
			return static_cast<UObject*>(Object);
	}

	return nullptr;
}

UObject* FUObjectArray::FindObjectFast(const char* Name)
{
	return FindObjectFast(FName(Name));
}

UObject* FUObjectArray::FindObjectFast(const std::string& Name)
{
	return FindObjectFast(FName(Name.c_str()));
}

UObject* FUObjectArray::DefaultObjImpl(const UClass* TargetClass)
{
	for (int i = 0; i < Num(); i++)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (Object && Object->IsDefaultObject() && Object->GetClass() == TargetClass)
			return Object;
	}
	return nullptr;
}

TArray<UObject*> FUObjectArray::GetObjectsOfClass(UClass* TargetClass, std::string MustIncludeString)
{
	TArray<UObject*> Objects;
	
	for (int i = 0; i < Num(); i++)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;
		if (!Object)
			continue;

		if (Object && Object->IsA(TargetClass)
			&& (MustIncludeString.empty() || Object->GetName().ToString().contains(MustIncludeString))) {
			Objects.Add(Object);
		}
	}

	return Objects;
}

TArray<UObject*> FUObjectArray::FindObjects(const std::string& SearchString)
{
	TArray<UObject*> Objects;

	std::string LowerSearch = Utils::StringToLower(SearchString);

	for (int i = 0; i < Num(); i++)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;
		if (!Object)
			continue;

		std::string objectFullName = Utils::StringToLower(Object->GetFullName());
		if (objectFullName.contains(LowerSearch))
		{
			Objects.Add(Object);
		}
	}

	return Objects;
}