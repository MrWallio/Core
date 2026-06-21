#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"

FString APlayerState::GetPlayerName() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetPlayerName");

	if (!Func) {
		return PlayerName;
	}

	struct PlayerState_GetPlayerName
	{
	public:
		FString ReturnValue;
	};

	PlayerState_GetPlayerName Parms{};

	const_cast<APlayerState*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}