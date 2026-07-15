#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/DataTableFunctionLibrary.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"

void UDataTableFunctionLibrary::EvaluateCurveTableRow(UCurveTable* CurveTable, FName RowName, float InXY, EEvaluateCurveTableResult::Type* OutResult, float* OutXY, const FString& ContextString)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.DataTableFunctionLibrary.EvaluateCurveTableRow");

	GetDefaultObj()->Call(Func, CurveTable, RowName, InXY, OutResult, OutXY, ContextString);
}

void UDataTableFunctionLibrary::GetDataTableRowNames(UDataTable* Table, TArray<FName>* OutRowNames)
{
	if (!OutRowNames)
		return;

	OutRowNames->Reset();

	if (Table)
	{
		*OutRowNames = Table->GetRowNames();
	}
}

bool UDataTableFunctionLibrary::DoesDataTableRowExist(UDataTable* Table, FName RowName)
{
	if (!Table)
		return false;

	return Table->GetRowMap().Contains(RowName);
}