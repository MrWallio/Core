#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

FString UKismetSystemLibrary::GetEngineVersion()
{
	if (Finder::FindUKismetSystemLibrary_GetEngineVersion() != 0) {
		FString(*GetEngineVersionInternal)() = decltype(GetEngineVersionInternal)(ImageBase + Finder::FindUKismetSystemLibrary_GetEngineVersion());
		return GetEngineVersionInternal();
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/Engine.KismetSystemLibrary.GetEngineVersion");

		if (!Func) {
			Log("UKismetSystemLibrary::GetEngineVersion: Failed to find function!");
			return FString();
		}
		else {
			struct KismetSystemLibrary_GetEngineVersion final
			{
			public:
				FString ReturnValue;
			};

			KismetSystemLibrary_GetEngineVersion Parms{};

			GetDefaultObj()->ProcessEvent(Func, &Parms);

			return Parms.ReturnValue;
		}
	}
}

void UKismetSystemLibrary::ExecuteConsoleCommand(UObject* WorldContextObject, const FString& Command, APlayerController* Player)
{
	if (Finder::FindUKismetSystemLibrary_ExecuteConsoleCommand() != 0) {
		void (*ExecuteConsoleCommandInternal)(UObject * WorldContextObject, const FString & Command, APlayerController * Player) = decltype(ExecuteConsoleCommandInternal)(ImageBase + Finder::FindUKismetSystemLibrary_ExecuteConsoleCommand());
		ExecuteConsoleCommandInternal(WorldContextObject, Command, Player);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");

		if (!Func) {
			Log("UKismetSystemLibrary::ExecuteConsoleCommand: Failed to find function!");
			return;
		}

		struct KismetSystemLibrary_ExecuteConsoleCommand final
		{
		public:
			UObject* WorldContextObject;
			FString Command;
			APlayerController* Player;
		};

		KismetSystemLibrary_ExecuteConsoleCommand Parms{};

		Parms.WorldContextObject = WorldContextObject;
		Parms.Command = std::move(Command);
		Parms.Player = Player;

		GetDefaultObj()->ProcessEvent(Func, &Parms);
	}
}