#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerController.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

void AFortPlayerController::ClientForceProfileQuery()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ClientForceProfileQuery"));

	ProcessEvent(Func, nullptr);
}

void AFortPlayerController::OnReadyToStartMatch(AFortPlayerController* This) {
	OnReadyToStartMatchOG(This);

	AFortPlayerControllerAthena* AthenaController = This->Cast<AFortPlayerControllerAthena>();
	if (AthenaController) {
		if ((!AthenaController->QuickBars || !AthenaController->ClientQuickBars))
		{
			AthenaController->SpawnQuickBars();
			AthenaController->SetupQuickBars();
		}
	}
}