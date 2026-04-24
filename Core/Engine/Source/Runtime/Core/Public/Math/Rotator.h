#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FRotator
{
public:
    DefineUnrealStruct(FRotator);
public:
    float Pitch;
    float Yaw;
    float Roll;

    /**
     * Default constructor (no initialization).
     */
    FRotator() {
        Pitch = 0.0f;
        Yaw = 0.0f;
        Roll = 0.0f;
    }
public:
    /**
     * Get Rotation as a quaternion.
     *
     * @return Rotation as a quaternion.
     */
    FQuat Quaternion() const;
};