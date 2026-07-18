#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Character.h"

void ACharacter::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("LaunchCharacter");

	Call(Func, LaunchVelocity, bXYOverride, bZOverride);
}
