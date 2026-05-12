#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

class FDebugDisplayInfo;
class UAIPerceptionComponent;
class UBehaviorTree;
class UBlackboardComponent;
class UBlackboardData;
class UBrainComponent;
class UCanvas;
class UGameplayTaskResource;
class UGameplayTasksComponent;
class UPawnAction;
class UPawnActionsComponent;
struct FVisualLogEntry;

class AAIController : public AController {
public:
	DefineUnrealClass(AAIController);
};