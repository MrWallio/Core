#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"

struct FSoftObjectPath
{
public:
	FName AssetPathName;
	FString SubPathString;
};