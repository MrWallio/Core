#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"

UPawnMovementComponent* APawn::GetMovementComponent() const
{
	return FindComponentByClass<UPawnMovementComponent>();
}

void APawn::OnRep_PlayerState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_PlayerState");

	Call(Func);
}