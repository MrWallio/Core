#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"

class UKismetStringLibrary : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UKismetStringLibrary);
public:
	/** Converts a string to a name value */
	static FName Conv_StringToName(const FString& InString);
};