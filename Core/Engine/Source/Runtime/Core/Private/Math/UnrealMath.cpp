#include "pch.h"
#include "../../Public/Math/UnrealMathUtility.h"
#include "../../Public/Math/Quat.h"
#include "../../Public/Math/Rotator.h"

FQuat FRotator::Quaternion() const
{
	FQuat& (*QuaternionInternal)(const FRotator*, FQuat&) = decltype(QuaternionInternal)(ImageBase + Finder::FindFRotator_Quaternion());
	FQuat Result = FQuat();
	QuaternionInternal(this, Result);
	return Result;
}