#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/PrimitiveComponent.h"

void UPrimitiveComponent::SetPhysicsLinearVelocity(const FVector& NewVel, bool bAddToCurrent, FName BoneName)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetPhysicsLinearVelocity");

	Call(Func, NewVel, bAddToCurrent, BoneName);
}

void UPrimitiveComponent::SetPhysicsAngularVelocityInRadians(const FVector& NewAngVel, bool bAddToCurrent, FName BoneName)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
	{
		Func = FindFunction("SetPhysicsAngularVelocityInRadians");
		if (!Func)
			Func = FindFunction("SetPhysicsAngularVelocity");
	}

	Call(Func, NewAngVel, bAddToCurrent, BoneName);
}
