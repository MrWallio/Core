#pragma once
#include "pch.h"

#include "FortAccountItem.h"

class UFortQuestItem : public UFortAccountItem {
public:
	DefineUnrealClass(UFortQuestItem);
public:
	bool HasCompletedQuest() const;
	bool HasCompletedObjectiveWithName(FName BackendName) const;
};