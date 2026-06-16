#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/DataTableFunctionLibrary.h"

struct FScalableFloat
{
public:
    DefineUnrealStructPath("ScriptStruct /Script/GameplayAbilities.ScalableFloat");

    float Value;
    uint8 Padding[0x4];
    FCurveTableRowHandle Curve;
    uint8 Padding2[0x10];

public:
    inline float Evaluate(float i = 0.f)
    {
        if (!Curve.CurveTable)
            return Value;

        float Out = 0.f;
        UDataTableFunctionLibrary::EvaluateCurveTableRow(Curve.CurveTable, Curve.RowName, i, nullptr, &Out, FString());
        return Value * Out;
    }
};