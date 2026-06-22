#include "pch.h"
#include "../Public/Abilities/GameplayAbilityTypes.h"
#include "../Public/GameplayAbilitySpec.h"

FGameplayAbilitySpec::FGameplayAbilitySpec(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel, int32 InInputID, UObject* InSourceObject)
{
	FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*, TSubclassOf<UGameplayAbility>, int32, int32, UObject*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecClassConstructor());
	ConstructorInternal(this, InAbilityClass, InLevel, InInputID, InSourceObject);
}

FGameplayAbilitySpec* FGameplayAbilitySpec::ConstructAbilitySpec(UGameplayAbility* InAbility, int32 InLevel, int32 InInputID, UObject* InSourceObject)
{
	FGameplayAbilitySpec* This = FGameplayAbilitySpec::Allocate();
	if (Finder::FindAbilitySpecCDOConstructor() > 0) {
		FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*, UGameplayAbility*, int32, int32, UObject*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecCDOConstructor());
		ConstructorInternal(This, InAbility, InLevel, InInputID, InSourceObject);
	}
	else {
		This->Ability = InAbility;
		This->Level = InLevel;
		This->InputID = InInputID;
		This->SourceObject = InSourceObject;
		This->ActiveCount = 0;
		This->InputPressed = false;
		This->RemoveAfterActivation = false;
		This->PendingRemove = false;
		This->MostRecentArrayReplicationKey = -1;
		This->ReplicationID = -1;
		This->ReplicationKey = -1;

		This->Handle.GenerateNewHandle();
	}

	return This;
}

TSharedPtr<FAbilityReplicatedDataCache> FGameplayAbilityReplicatedDataContainer::Find(const FGameplayAbilitySpecHandleAndPredictionKey& Key) const
{
	for (const FKeyDataPair& Pair : InUseData)
	{
		if (Pair.Key() == Key)
		{
			return Pair.Value();
		}
	}

	return TSharedPtr<FAbilityReplicatedDataCache>();
}