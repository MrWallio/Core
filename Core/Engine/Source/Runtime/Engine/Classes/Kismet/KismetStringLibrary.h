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
	static FName Conv_StringToName(const FString& InString);

	static FString ToUpper(const FString& SourceString);

	static bool Contains(const FString& SearchIn, const FString& Substring, bool bUseCase, bool bSearchFromEnd);

	static int32 FindSubstring(const FString& SearchIn, const FString& Substring, bool bUseCase, bool bSearchFromEnd, int32 StartPosition);

	static FString Left(const FString& SourceString, int32 Count);
};