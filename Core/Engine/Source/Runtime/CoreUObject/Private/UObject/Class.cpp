#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"

UClass* UField::GetOwnerClass() const
{
	for (UObject* Outer = GetOuter(); Outer; Outer = Outer->GetOuter())
	{
		if (UClass* Class = Outer->Cast<UClass>())
			return Class;
	}
	return nullptr;
}

UStruct* UField::GetOwnerStruct() const
{
	for (UObject* Obj = (UObject*)this; Obj; Obj = Obj->GetOuter())
	{
		if (UStruct* Struct = Obj->Cast<UStruct>())
			return Struct;
	}
	return nullptr;
}

UProperty* UStruct::FindPropertyByName(FName InName) const
{
	for (UField* Field = GetChildrenOrChildProperties(); Field; Field = Field->Next)
	{
		UProperty* Prop = (UProperty*)Field;
		if (Prop && Prop->GetFName() == InName)
			return Prop;
	}

	if (SuperStruct)
		return SuperStruct->FindPropertyByName(InName);

	return nullptr;
}

uintptr_t UStruct::GetPropertyOffset(std::string InName) const
{
	FString PropertyNameString = FString(std::wstring(InName.begin(), InName.end()).c_str());
	FName PropertyName = UKismetStringLibrary::Conv_StringToName(PropertyNameString);
	UProperty* Prop = FindPropertyByName(PropertyName);
	if (!Prop) {
		return -1;
	}
	return Prop->Offset_Internal;
}

bool UStruct::IsChildOf(const UStruct* SomeBase) const
{
	if (!SomeBase)
		return false;

	for (const UStruct* Struct = this; Struct; Struct = Struct->SuperStruct)
	{
		if (Struct == SomeBase)
			return true;
	}

	return false;
}

int64 UEnum::GetValue(const char* EnumMemberName) const
{
	if (!this)
		return -1;

	auto Names = *(TArray<TPair<FName, int64>>*)(__int64(this) + 0x40);

	for (int i = 0; i < Names.Num(); i++)
	{
		auto& Pair = Names[i];
		auto& Name = Pair.Key();
		auto& Value = Pair.Value();

		if (Name.ComparisonIndex)
		{
			auto str = Name.ToString().ToString();
			auto colcolIdx = str.find_last_of("::");

			auto RealName = colcolIdx == -1 ? str : str.substr(colcolIdx + 1);

			if (RealName == EnumMemberName)
				return Value;
		}
	}

	return -1;
}

int32 UEnum::NumEnums() const
{
	if (!this)
		return 0;

	auto Names = *(TArray<TPair<FName, int64>>*)(__int64(this) + 0x40);
	return Names.Num();
}

std::string UEnum::GetNameStringByValue(int64 Value) const
{
	if (!this)
		return "None";

	auto Names = *(TArray<TPair<FName, int64>>*)(__int64(this) + 0x40);

	for (int i = 0; i < Names.Num(); i++)
	{
		auto& Pair = Names[i];
		if (Pair.Value() != Value)
			continue;

		auto str = Pair.Key().ToString().ToString();
		auto colcolIdx = str.find_last_of("::");
		return colcolIdx == -1 ? str : str.substr(colcolIdx + 1);
	}

	return "None";
}

int64 UEnum::GetValueByIndex(int32 Index) const
{
	if (!this)
		return -1;

	auto Names = *(TArray<TPair<FName, int64>>*)(__int64(this) + 0x40);

	if (Index < 0 || Index >= Names.Num())
		return -1;

	return Names[Index].Value();
}

UFunction* UClass::FindFunctionByName(FName InName, EIncludeSuperFlag::Type IncludeSuper) const
{
	for (const UStruct* Struct = this; Struct; Struct = Struct->SuperStruct)
	{
		for (UField* Field = Struct->Children; Field; Field = Field->Next)
		{
			if (Field->IsA(EClassCastFlags::CASTCLASS_UFunction) && Field->GetFName() == InName)
				return (UFunction*)Field;
		}

		if (IncludeSuper == EIncludeSuperFlag::ExcludeSuper)
			break;
	}

	return nullptr;
}

UFunction* UClass::GetFunction(const std::string& ClassName, const std::string& FuncName) const
{
	for (const UStruct* Clss = this; Clss; Clss = Clss->SuperStruct)
	{
		if (Clss->GetFName().ToString().ToString() != ClassName)
			continue;

		for (UField* Field = Clss->Children; Field; Field = Field->Next)
		{
			if (Field->IsA(EClassCastFlags::CASTCLASS_UFunction) && Field->GetFName().ToString().ToString() == FuncName)
				return static_cast<class UFunction*>(Field);
		}
	}

	return nullptr;
}

UFunction* UClass::GetFunction(const std::string& FullName, bool bSilent) const
{
	UObject* Function = FUObjectArray::FindObject(FullName);
	if (!Function) {
		if (!bSilent) {
			Log("Failed to find function with full name: " + FullName);
		}
		return nullptr;
	}
	return (UFunction*)Function;
}

const void* UClass::GetSparseClassData(const EGetSparseClassDataMethod GetMethod)
{
	const void* (*GetSparseClassDataInternal)(const UClass*, const EGetSparseClassDataMethod) = decltype(GetSparseClassDataInternal)(ImageBase + Finder::FindUClass_GetSparseClassData());
	return GetSparseClassDataInternal(this, GetMethod);
}