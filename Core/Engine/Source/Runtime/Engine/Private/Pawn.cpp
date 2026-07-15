#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

UPawnMovementComponent* APawn::GetMovementComponent() const
{
	return FindComponentByClass<UPawnMovementComponent>();
}

template AController* APawn::GetController<AController>() const;
template APlayerState* APawn::GetPlayerState<APlayerState>() const;

void APawn::OnRep_PlayerState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_PlayerState");

	Call(Func);
}