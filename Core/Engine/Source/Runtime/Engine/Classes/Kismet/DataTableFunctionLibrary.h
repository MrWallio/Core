#pragma once
#include "pch.h"

#include "BlueprintFunctionLibrary.h"

class UCurveTable;

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
};