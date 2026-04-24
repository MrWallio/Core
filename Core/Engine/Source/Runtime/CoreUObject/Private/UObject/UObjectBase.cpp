#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBase.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"

void UObjectBase::EmitBaseReferences(UClass* RootClass)
{
	void (*EmitBaseReferencesInternal)(UClass*) = decltype(EmitBaseReferencesInternal)(ImageBase + Finder::FindUObjectBase_EmitBaseReferences());
	EmitBaseReferencesInternal(RootClass);
}

void UObjectBase::LowLevelRename(FName NewName, UObject* NewOuter)
{
	void (*LowLevelRenameInternal)(UObjectBase*, FName, UObject*) = decltype(LowLevelRenameInternal)(ImageBase + Finder::FindUObjectBase_LowLevelRename());
	LowLevelRenameInternal(this, NewName, NewOuter);
}

void UObjectBase::Register(const TCHAR* PackageName, const TCHAR* InName)
{
	void (*RegisterInternal)(UObjectBase*, const TCHAR*, const TCHAR*) = decltype(RegisterInternal)(ImageBase + Finder::FindUObjectBase_Register());
	RegisterInternal(this, PackageName, InName);
}

void UObjectBase::DeferredRegister(UClass* UClassStaticClass, const TCHAR* PackageName, const TCHAR* InName)
{
	void (*DeferredRegisterInternal)(UObjectBase*, UClass*, const TCHAR*, const TCHAR*) = decltype(DeferredRegisterInternal)(ImageBase + Finder::FindUObjectBase_DeferredRegister());
	DeferredRegisterInternal(this, UClassStaticClass, PackageName, InName);
}

bool UObjectBase::IsValidLowLevel() const
{
	bool (*IsValidLowLevelInternal)(const UObjectBase*) = decltype(IsValidLowLevelInternal)(ImageBase + Finder::FindUObjectBase_IsValidLowLevel());
	return IsValidLowLevelInternal(this);
}

bool UObjectBase::IsValidLowLevelFast(bool bRecursive) const
{
	bool (*IsValidLowLevelFastInternal)(const UObjectBase*, bool) = decltype(IsValidLowLevelFastInternal)(ImageBase + Finder::FindUObjectBase_IsValidLowLevelFast());
	return IsValidLowLevelFastInternal(this, bRecursive);
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
	if (!this || !TypeClass)
		return false;
	if (!IsValidLowLevelFast())
		return false;

	if (!GetClass())
		return false;

	return GetClass()->IsSubclassOf(TypeClass);
}

UProperty* UObjectBase::FindPropertyByName(std::string InName) const
{
	return GetClass() ? GetClass()->FindPropertyByName(InName) : nullptr;
}