#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

class AActor;
class UActorComponent;
class UAssetUserData;
class ULevel;
class UWorld;
class UPrimitiveComponent;

class UActorComponent : public UObject {
public:
	DefineUnrealClass(UActorComponent);
public:
	AActor* GetOwner() const;
};

FORCEINLINE AActor* UActorComponent::GetOwner() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetOwner"));

	struct ActorComponent_GetOwner final
	{
	public:
		AActor* ReturnValue;
	};

	ActorComponent_GetOwner Parms{};

	const_cast<UActorComponent*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}