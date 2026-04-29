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

void APlayerController::ClientMessage(const FString& S, FName Type, float MsgLifeTime)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName("ClientMessage"));

	struct PlayerController_ClientMessage final
	{
	public:
		FString S;
		FName Type;
		float MsgLifeTime;
	};

	PlayerController_ClientMessage Parms{};

	Parms.S = std::move(S);
	Parms.Type = Type;
	Parms.MsgLifeTime = MsgLifeTime;

	ProcessEvent(Func, &Parms);
}

void APlayerController::ClientMessage(const FString& S)
{
	ClientMessage(S, UKismetStringLibrary::Conv_StringToName("None"), 0.f);
}

void APlayerController::ClientTeamMessage(APlayerState* SenderPlayerState, const FString& S, FName Type, float MsgLifeTime)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName("ClientTeamMessage"));

	struct PlayerController_ClientTeamMessage final
	{
	public:
		APlayerState* SenderPlayerState;
		FString S;
		FName Type;
		float MsgLifeTime;
	};

	PlayerController_ClientTeamMessage Parms{};

	Parms.SenderPlayerState = SenderPlayerState;
	Parms.S = std::move(S);
	Parms.Type = Type;
	Parms.MsgLifeTime = MsgLifeTime;

	ProcessEvent(Func, &Parms);
}