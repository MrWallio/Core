#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

UFunction* UObject::FindFunction(FName InName) const
{
	return GetClass()->FindFunctionByName(InName, EIncludeSuperFlag::IncludeSuper);
}

UFunction* UObject::FindFunction(std::string InName) const
{
	return FindFunction(UKismetStringLibrary::Conv_StringToName(InName));
}

UObject* UObject::GetArchetypeFromRequiredInfo(UClass* Class, UObject* Outer, FName Name, EObjectFlags ObjectFlags)
{
	UObject* (*GetArchetypeFromRequiredInfoInternal)(UClass*, UObject*, FName, EObjectFlags) = decltype(GetArchetypeFromRequiredInfoInternal)(ImageBase + Finder::FindUObject_GetArchetypeFromRequiredInfo());
	return GetArchetypeFromRequiredInfoInternal(Class, Outer, Name, ObjectFlags);
}

void UObject::ProcessEvent(UFunction* Function, void* Parms)
{
	void (*&ProcessEventInternal)(UObject*, UFunction*, void*) = decltype(ProcessEventInternal)(VTable[Finder::FindProcessEventVFT()]);
	ProcessEventInternal(this, Function, Parms);
}

UWorld* UObject::GetWorld() const
{
	if (Finder::FindUObject_GetWorldVFT()) {
		UWorld* (*&GetWorldInternal)(const UObject*) = decltype(GetWorldInternal)(VTable[Finder::FindUObject_GetWorldVFT()]);
		return GetWorldInternal(this);
	}
	else {
		return UWorld::GetWorld();
	}
}

bool UObject::NeedsLoadForClient(UObject* This)
{
	return true;
}

void UObject::Hook() {
	HookEveryVTableIdx(
		UObject::StaticClass(),
		Finder::FindUObject_NeedsLoadForClientVFT(),
		NeedsLoadForClient,
		nullptr,
		true
	);
	
	Log("Hooked UObject");
}