#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Interface.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class IGameplayTagAssetInterface : public IInterface {
public:
	DefineUnrealClass(IGameplayTagAssetInterface);

	void GetOwnedGameplayTags(FGameplayTagContainer* TagContainer) const {
		void (*&GetOwnedGameplayTagsInternal)(const IGameplayTagAssetInterface*, FGameplayTagContainer*) = decltype(GetOwnedGameplayTagsInternal)(VTable[0x2]);
		GetOwnedGameplayTagsInternal(this, TagContainer);
	}
};
