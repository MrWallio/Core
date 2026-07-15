#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class ULevel;
class UWorld;

class ULevelStreaming : public UObject {
public:
	DefineUnrealClass(ULevelStreaming);

	DefineUProperty(ULevel*, LoadedLevel);
	DefineUProperty(ULevel*, PendingUnloadLevel);
	DefineUProperty(FName, PackageNameToLoad);
	DefineBitfieldUProperty(bShouldBeLoaded);
	DefineBitfieldUProperty(bShouldBeVisible);
	DefineBitfieldUProperty(bIsStatic);
	DefineBitfieldUProperty(bShouldBlockOnLoad);
public:
	FORCEINLINE ULevel* GetLoadedLevel() const { return LoadedLevel; }
	FORCEINLINE bool ShouldBeLoaded() const { return bShouldBeLoaded; }
	FORCEINLINE bool ShouldBeVisible() const { return bShouldBeVisible; }
	FORCEINLINE bool IsLevelLoaded() const { return LoadedLevel != nullptr; }
};

class ULevelStreamingKismet : public ULevelStreaming {
public:
	DefineUnrealClass(ULevelStreamingKismet);
};

class ULevelStreamingDynamic : public ULevelStreaming {
public:
	DefineUnrealClass(ULevelStreamingDynamic);
};

class ULevelStreamingAlwaysLoaded : public ULevelStreaming {
public:
	DefineUnrealClass(ULevelStreamingAlwaysLoaded);
};
