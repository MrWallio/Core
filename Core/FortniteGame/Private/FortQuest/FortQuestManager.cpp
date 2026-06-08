#include "pch.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"

#include "FortniteGame/Public/FortItem/FortQuestItem.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveStatTableRow.h"
#include "FortniteGame/Public/Info/FortRegisteredPlayerInfo.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

void UFortQuestManager::SendCustomStatEvent(UFortQuestManager* This, FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush) {
	return SendCustomStatEventOG(This, ObjectiveStat, Count, bForceFlush);
}

void UFortQuestManager::SendStatEvent(
	UFortQuestManager* This,
	FScriptContainerElement* InObjectiveStat,
	uint8 InType,
	UObject* InTargetObject,
	FGameplayTagContainer* InTargetTags,
	FGameplayTagContainer* InSourceTags,
	FGameplayTagContainer* InContextTags,
	int32 InCount,
	bool bForceFlush
) {
	/*Log("FortQuestManager: " + This->GetName().ToString());
	Log("InObjectiveStat: " + std::to_string((uintptr_t)InObjectiveStat));
	Log("InType: " + std::to_string(InType));
	Log("InTargetObject: " + (InTargetObject ? InTargetObject->GetName().ToString() : "nullptr"));
	for (int32 i = 0; i < InTargetTags->Num(); i++) {
		Log("InTargetTags[" + std::to_string(i) + "]: " + InTargetTags->GetByIndex(i).ToString().ToString());
	}
	for (int32 i = 0; i < InSourceTags->Num(); i++) {
		Log("InSourceTags[" + std::to_string(i) + "]: " + InSourceTags->GetByIndex(i).ToString().ToString());
	}
	for (int32 i = 0; i < InContextTags->Num(); i++) {
		Log("InContextTags[" + std::to_string(i) + "]: " + InContextTags->GetByIndex(i).ToString().ToString());
	}
	Log("InCount: " + std::to_string(InCount));
	Log("bForceFlush: " + std::string(bForceFlush ? "true" : "false"));*/

	AFortPlayerController* PlayerController = This->GetPlayerControllerBP();
	if (!PlayerController) {
		Log("UFortQuestManager::SendStatEvent: PlayerController is null!");
		return SendStatEventOG(This, InObjectiveStat, InType, InTargetObject, InTargetTags, InSourceTags, InContextTags, InCount, bForceFlush);
	}

	for (UFortQuestItem* QuestItem : This->CurrentQuests) {
		if (QuestItem->HasCompletedQuest())
			continue;

		UFortQuestItemDefinition* QuestDefinition = QuestItem->ItemDefinition->Cast<UFortQuestItemDefinition>();
		if (!QuestDefinition)
			continue;

		for (int32 i = 0; i < QuestDefinition->Objectives.Num(); i++) {
			FFortMcpQuestObjectiveInfo& Objective = QuestDefinition->Objectives.GetWithSize(i, FFortMcpQuestObjectiveInfo::GetSize());
			if (QuestItem->HasCompletedObjectiveWithName(Objective.BackendName))
				continue;

			FDataTableRowHandle ObjectiveStatHandle = Objective.ObjectiveStatHandle;
			if (!Objective.ObjectiveStatHandle.RowName.IsNone() && ObjectiveStatHandle.DataTable) {
				for (int32 j = 0; j < ObjectiveStatHandle.DataTable->RowMap.Num(); j++) {
					auto& Row = ObjectiveStatHandle.DataTable->RowMap[j];

					FName& Key = Row.Key();

					if (Key != ObjectiveStatHandle.RowName) {
						continue;
					}

					FFortQuestObjectiveStatTableRow* Value = (FFortQuestObjectiveStatTableRow*)Row.Value();

					if (Value->Type != InType) {
						continue;
					}

					if (!InTargetTags->HasAll(Value->TargetTagContainer)) {
						continue;
					}

					if (!InSourceTags->HasAll(Value->SourceTagContainer)) {
						continue;
					}

					/*if (!InContextTags->HasAll(Value->ContextTagContainer)) {
						continue;
					}*/

					Log("ObjectiveStatHandle Row: " + Row.Key().ToString().ToString());
				}
			}
		}
	}

	return SendStatEventOG(This, InObjectiveStat, InType, InTargetObject, InTargetTags, InSourceTags, InContextTags, InCount, bForceFlush);
}

FScriptContainerElement* UFortQuestManager::ProcessPendingStatEvents() {
	FScriptContainerElement* (*ProcessPendingStatEventsInternal)(UFortQuestManager*) = decltype(ProcessPendingStatEventsInternal)(ImageBase + Finder::FindUFortQuestManager_ProcessPendingStatEvents());
	return ProcessPendingStatEventsInternal(this);
}

AFortPlayerController* UFortQuestManager::GetPlayerControllerBP() {
	if (Version::Fortnite_Version <= 1.8) {
		UWorld* World = UWorld::GetWorld();
		if (!World) {
			Log("UFortQuestManager::GetPlayerControllerBP: World is null!");
			return nullptr;
		}

		UFortRegisteredPlayerInfo* PlayerInfo = OuterPrivate->Cast<UFortRegisteredPlayerInfo>();
		if (!PlayerInfo) {
			Log("UFortQuestManager::GetPlayerControllerBP: PlayerInfo is null!");
			return nullptr;
		}

		TArray<AActor*> PlayerControllers;
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &PlayerControllers);

		for (AActor* Actor : PlayerControllers) {
			AFortPlayerController* PC = Actor->Cast<AFortPlayerController>();
			if (!PC)
				continue;
			
			if (PC->GetRegisteredPlayerInfo() != PlayerInfo)
				continue;

			return PC;
		}
	}
	else {
		// sdk
	}

	return nullptr;
}