#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/DataTableFunctionLibrary.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"

void UDataTableFunctionLibrary::EvaluateCurveTableRow(UCurveTable* CurveTable, FName RowName, float InXY, EEvaluateCurveTableResult::Type* OutResult, float* OutXY, const FString& ContextString)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.DataTableFunctionLibrary.EvaluateCurveTableRow");

	struct DataTableFunctionLibrary_EvaluateCurveTableRow final
	{
	public:
		UCurveTable* CurveTable;
		FName RowName;
		float InXY;
		EEvaluateCurveTableResult::Type OutResult;
		float OutXY;
		FString ContextString;
	};

	DataTableFunctionLibrary_EvaluateCurveTableRow Parms{};

	Parms.CurveTable = CurveTable;
	Parms.RowName = RowName;
	Parms.InXY = InXY;
	Parms.ContextString = std::move(ContextString);

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	if (OutResult != nullptr)
		*OutResult = Parms.OutResult;

	if (OutXY != nullptr)
		*OutXY = Parms.OutXY;
}