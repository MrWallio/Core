#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class FArchive;
class FCbWriter;
class FLinkerInstancingContext;
class FObjectPreSaveContext;
class FOutputDevice;
class FPackagePath;
class ITargetPlatform;
class UClass;
class UEnum;
class UFunction;
class UObject;
class UObjectBase;
class UPackage;
class UPackageMap;
class UScriptStruct;
class UWorld;
struct FCustomPropertyListNode;
struct FGuid;
struct FObjectInstancingGraph;
struct FObjectPostCDOCompiledContext;
struct FObjectPtr;
struct FPrimaryAssetId;
struct FStaticConstructObjectParameters;
struct FUObjectSerializeContext;
struct FWorldContext;
template <typename T>
struct TObjectPtr;

void BeginLoad(FUObjectSerializeContext* LoadContext, const TCHAR* DebugContext = nullptr);
void EndLoad(FUObjectSerializeContext* LoadContext);