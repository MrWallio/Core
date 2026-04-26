#include "pch.h"
#include "../Public/Abilities/GameplayAbilityTypes.h"
#include "../Public/GameplayAbilitySpec.h"

FGameplayAbilitySpec::FGameplayAbilitySpec(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel, int32 InInputID, UObject* InSourceObject)
{
	std::memset(this, 0, GetSize());
	FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*, TSubclassOf<UGameplayAbility>, int32, int32, UObject*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecClassConstructor());
	ConstructorInternal(this, InAbilityClass, InLevel, InInputID, InSourceObject);
}

FGameplayAbilitySpec::FGameplayAbilitySpec(UGameplayAbility* InAbility, int32 InLevel, int32 InInputID, UObject* InSourceObject)
{
	std::memset(this, 0, GetSize());
	if (Finder::FindAbilitySpecCDOConstructor()) {
		FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*, UGameplayAbility*, int32, int32, UObject*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecCDOConstructor());
		ConstructorInternal(this, InAbility, InLevel, InInputID, InSourceObject);
	}
	else {
		Ability = InAbility;
		Level = InLevel;
		InputID = InInputID;
		SourceObject = InSourceObject;
		ActiveCount = 0;
		InputPressed = false;
		RemoveAfterActivation = false;
		PendingRemove = false;

		Handle.GenerateNewHandle();
	}
}