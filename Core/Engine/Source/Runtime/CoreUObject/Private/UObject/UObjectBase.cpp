#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBase.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"

bool UObjectBase::IsValidLowLevel() const
{
	if (this == nullptr || ClassPrivate == nullptr)
		return false;

	FUObjectItem* Item = FUObjectArray::IndexToObject(InternalIndex);
	return Item && Item->Object == this;
}

bool UObjectBase::IsValidLowLevelFast(bool bRecursive) const
{
	if (Finder::FindUObjectBase_IsValidLowLevelFast()) {
		bool (*IsValidLowLevelFastInternal)(const UObjectBase*, bool) = decltype(IsValidLowLevelFastInternal)(ImageBase + Finder::FindUObjectBase_IsValidLowLevelFast());
		return IsValidLowLevelFastInternal(this, bRecursive);
	}

	return true;
}

std::string UObjectBase::GetFullName() const
{
	UClass* Class = GetClass();
	if (this && Class)
	{
		std::string Temp;

		FName ObjectName = GetFName();
		FString ObjectFString = ObjectName.ToString();
		std::string ObjectString = ObjectFString.ToString();

		for (UObjectBase* NextOuter = OuterPrivate; NextOuter; NextOuter = NextOuter->OuterPrivate)
		{
			FName OuterFName = NextOuter->GetFName();
			FString OuterFString = OuterFName.ToString();
			std::string OuterString = OuterFString.ToString();
			Temp = OuterString + "." + Temp;
		}

		FName ClassName = Class->GetFName();
		FString ClassString = ClassName.ToString();
		std::string Name = ClassString.ToString();
		Name += " ";
		Name += Temp;
		Name += ObjectString;

		return Name;
	}

	return "None";
}

bool UObjectBase::IsA(EClassCastFlags TypeFlags) const
{
	return (ClassPrivate->GetCastFlags() & TypeFlags);
}

bool UObjectBase::IsA(UClass* TypeClass) const
{
	if (!this) {
		return false;
	}

	if (!TypeClass) {
		return false;
	}

	if (!IsValidLowLevelFast()) {
		Log("IsA: Object is not valid: " + GetFullName());
		return false;
	}

	if (!GetClass()) {
		Log("IsA: Object has no class: " + GetFullName());
		return false;
	}

	return GetClass()->IsSubclassOf(TypeClass);
}

UProperty* UObjectBase::FindPropertyByName(FName InName) const
{
	return GetClass() ? GetClass()->FindPropertyByName(InName) : nullptr;
}