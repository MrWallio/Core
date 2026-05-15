#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"

#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/FortAnalytics/AnalyticsProvider.h"

class UFortGameInstance : public UGameInstance {
public:
	DefineUnrealClass(UFortGameInstance);
public:
	static void Hook() {
		Log("UFortGameInstance Hooked!");
	}
};