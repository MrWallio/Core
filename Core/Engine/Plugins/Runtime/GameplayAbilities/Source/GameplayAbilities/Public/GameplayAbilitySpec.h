#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "GameplayAbilitySpecHandle.h"

class UAbilitySystemComponent;
class UGameplayAbility;

struct FGameplayAbilitySpec : public FFastArraySerializerItem {
public:
	DefineUnrealStruct(FGameplayAbilitySpec);

	DefineStructProperty(FGameplayAbilitySpecHandle, Handle);

	DefineStructProperty(UGameplayAbility*, Ability);

	DefineBitfieldStructProperty(InputPressed);

	DefineStructProperty(int32, Level);

	DefineStructProperty(int32, InputID);

	DefineStructProperty(UObject*, SourceObject);

	DefineStructProperty(uint8, ActiveCount);

	DefineBitfieldStructProperty(RemoveAfterActivation);

	DefineBitfieldStructProperty(PendingRemove);
public:
	FGameplayAbilitySpec()
	{
		std::memset(this, 0, GetSize());
		static FGameplayAbilitySpec* (*ConstructorInternal)(FGameplayAbilitySpec*) = decltype(ConstructorInternal)(ImageBase + Finder::FindAbilitySpecDefaultConstructor());
		ConstructorInternal(this);
	}

	/** Version that takes an ability class */
	FGameplayAbilitySpec(TSubclassOf<UGameplayAbility> InAbilityClass, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject* InSourceObject = nullptr);

	/** Version that takes an ability CDO, this exists for backward compatibility */
	FGameplayAbilitySpec(UGameplayAbility* InAbility, int32 InLevel = 1, int32 InInputID = INDEX_NONE, UObject* InSourceObject = nullptr);
};

struct FGameplayAbilitySpecContainer : public FFastArraySerializer {
public:
	DefineUnrealStruct(FGameplayAbilitySpecContainer);

	DefineStructProperty(TArray<FGameplayAbilitySpec>, Items);
	DefineStructProperty(UAbilitySystemComponent*, Owner);
};