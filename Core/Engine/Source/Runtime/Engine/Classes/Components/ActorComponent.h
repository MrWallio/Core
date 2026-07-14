#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class AActor;
class UActorComponent;
class UAssetUserData;
class ULevel;
class UWorld;
class UPrimitiveComponent;

class UActorComponent : public UObject {
public:
	DefineUnrealClass(UActorComponent);

	DefineUProperty(TArray<FName>, ComponentTags);
	DefineBitfieldUProperty(bReplicates);
	DefineBitfieldUProperty(bAutoActivate);
	DefineBitfieldUProperty(bIsActive);
public:
	FORCEINLINE AActor* GetOwner() const { return GetTypedOuter<AActor>(); }

	FORCEINLINE bool IsActive() const { return bIsActive; }
	FORCEINLINE bool GetIsReplicated() const { return bReplicates; }
};
