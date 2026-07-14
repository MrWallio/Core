#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Level.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"

TArray<AActor*> ULevel::GetActors() const
{
	TArray<AActor*> Result;
	for (AActor* Actor : TObjectRange<AActor>())
	{
		if (Actor->GetOuter() == this)
		{
			Result.Add(Actor);
		}
	}
	return Result;
}

bool ULevel::IsPersistentLevel() const
{
	bool bIsPersistent = false;
	if (OwningWorld)
	{
		bIsPersistent = (this == OwningWorld->PersistentLevel);
	}
	return bIsPersistent;
}

bool ULevel::HasVisibilityChangeRequestPending() const
{
	return (OwningWorld && (this == OwningWorld->GetCurrentLevelPendingVisibility() || this == OwningWorld->GetCurrentLevelPendingInvisibility()));
}

ALevelScriptActor* ULevel::GetLevelScriptActor() const
{
	return LevelScriptActor;
}

bool ULevel::HasVisibilityRequestPending() const
{
	return (OwningWorld && this == OwningWorld->GetCurrentLevelPendingVisibility());
}

bool ULevel::IsCurrentLevel() const
{
	bool bIsCurrent = false;
	if (OwningWorld)
	{
		bIsCurrent = (this == OwningWorld->GetCurrentLevel());
	}
	return bIsCurrent;
}