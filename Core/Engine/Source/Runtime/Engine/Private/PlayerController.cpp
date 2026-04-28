#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

void APlayerController::ServerAcknowledgePossession(APawn* P)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ServerAcknowledgePossession"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ServerAcknowledgePossessionInternal)(APlayerController*, APawn*) = decltype(ServerAcknowledgePossessionInternal)(VTable[VTableIdx]);
		ServerAcknowledgePossessionInternal(this, P);
	}
}

void APlayerController::SendClientAdjustment()
{
	void (*&SendClientAdjustmentInternal)(APlayerController*) = decltype(SendClientAdjustmentInternal)(VTable[Finder::FindAPlayerController_SendClientAdjustmentVFT()]);
	SendClientAdjustmentInternal(this);
}