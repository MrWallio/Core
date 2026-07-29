#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/SceneComponent.h"

class AActor;

class UChildActorComponent : public USceneComponent {
public:
	DefineUnrealClass(UChildActorComponent);

	DefineUProperty(TSubclassOf<AActor>, ChildActorClass);
	DefineUProperty(AActor*, ChildActor);
};
