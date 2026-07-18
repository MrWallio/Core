#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"

void UActorComponent::SetIsReplicated(bool bShouldReplicate)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetIsReplicated");

	Call(Func, bShouldReplicate);
}
