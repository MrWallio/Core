#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UCurveTable : public UObject {
public:
	DefineUnrealClass(UCurveTable);
};

struct FCurveTableRowHandle {
public:
	DefineUnrealStruct(FCurveTableRowHandle);

	DefineStructProperty(UCurveTable*, CurveTable);

	DefineStructProperty(FName, RowName);
};