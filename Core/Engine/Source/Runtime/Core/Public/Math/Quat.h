#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Rotator.h"

class Error;

struct alignas(0x10) FQuat
{
public:
    DefineUnrealStruct(FQuat);
public:
    float X;
    float Y;
    float Z;
    float W;
public:
    /** Default constructor (no initialization). */
    FORCEINLINE FQuat() {}

    /**
     * Creates and initializes a new quaternion from the given rotator.
     *
     * @param R The rotator to initialize from.
     */
    FQuat(const FRotator& R);
};

FORCEINLINE FQuat::FQuat(const FRotator& R)
{
    *this = R.Quaternion();
}