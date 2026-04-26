#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"

class AActor;
class Error;
class FOutBunch;
class UActorChannel;

class UGameplayTasksComponent : public UActorComponent {
public:
	DefineUnrealClass(UGameplayTasksComponent);
};