#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

void AController::Possess(APawn* InPawn)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"Possess"));

	struct Controller_Possess
	{
	public:
		APawn* InPawn;
	};

	Controller_Possess Parms{};

	Parms.InPawn = InPawn;

	ProcessEvent(Func, &Parms);
}