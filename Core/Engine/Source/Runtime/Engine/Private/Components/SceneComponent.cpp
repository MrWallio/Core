#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/SceneComponent.h"

void USceneComponent::SetRelativeScale3D(FVector NewScale3D)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetRelativeScale3D");

	Call(Func, NewScale3D);
}
