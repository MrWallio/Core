#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Quat.h"
#include "Vector.h"

struct FTransform
{
public:
    DefineUnrealStruct(FTransform);
public:
    FQuat Rotation;
    FVector Translation;
    uint8 Pad_1C[0x4];
    FVector Scale3D;
    uint8 Pad_2C[0x4];
};