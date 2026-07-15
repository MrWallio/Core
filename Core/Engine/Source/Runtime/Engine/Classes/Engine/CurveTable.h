#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"

class UCurveTable : public UObject {
public:
	DefineUnrealClass(UCurveTable);

	TMap<FName, uint8*> RowMap;
public:
	FORCEINLINE const TMap<FName, uint8*>& GetRowMap() const { return RowMap; }

	uint8* FindCurveUnchecked(FName RowName) const
	{
		return RowName == FName() ? nullptr : RowMap.FindRef(RowName);
	}

	TArray<FName> GetRowNames() const
	{
		TArray<FName> Keys;
		RowMap.GenerateKeyArray(Keys);
		return Keys;
	}
};

struct FCurveTableRowHandle {
public:
	DefineUnrealStruct(FCurveTableRowHandle);

	DefineStructProperty(UCurveTable*, CurveTable);

	DefineStructProperty(FName, RowName);
};