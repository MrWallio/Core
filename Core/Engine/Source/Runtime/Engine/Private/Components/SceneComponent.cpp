#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/SceneComponent.h"

#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"

void USceneComponent::SetRelativeScale3D(FVector NewScale3D)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetRelativeScale3D");

	Call(Func, NewScale3D);
}

void USceneComponent::K2_SetWorldTransform(const FTransform& NewTransform, bool bSweep, FHitResult* SweepHitResult, bool bTeleport)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("K2_SetWorldTransform");

	Call(Func, NewTransform, bSweep, SweepHitResult, bTeleport);
}
