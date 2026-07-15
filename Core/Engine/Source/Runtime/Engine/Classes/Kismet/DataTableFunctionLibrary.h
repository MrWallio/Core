#pragma once
#include "pch.h"

#include "BlueprintFunctionLibrary.h"

class UCurveTable;
class UDataTable;

/** Enum used to indicate success or failure of EvaluateCurveTableRow. */
namespace EEvaluateCurveTableResult
{
    enum Type
    {
        /** Found the row successfully. */
        RowFound,
        /** Failed to find the row. */
        RowNotFound,
    };
}

class UDataTableFunctionLibrary : public UBlueprintFunctionLibrary {
public:
    DefineUnrealClass(UDataTableFunctionLibrary);
public:
    static void EvaluateCurveTableRow(UCurveTable* CurveTable, FName RowName, float InXY, EEvaluateCurveTableResult::Type* OutResult, float* OutXY, const FString& ContextString);

    static void GetDataTableRowNames(UDataTable* Table, TArray<FName>* OutRowNames);

    static bool DoesDataTableRowExist(UDataTable* Table, FName RowName);
};