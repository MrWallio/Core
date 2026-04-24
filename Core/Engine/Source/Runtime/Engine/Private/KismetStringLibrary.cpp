#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

FName UKismetStringLibrary::Conv_StringToName(const FString& InString)
{
	if (Finder::FindUKismetStringLibrary_Conv_StringToName() != 0) {
		FName(*Conv_StringToNameInternal)(const FString&) = decltype(Conv_StringToNameInternal)(ImageBase + Finder::FindUKismetStringLibrary_Conv_StringToName());
		return Conv_StringToNameInternal(InString);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.Conv_StringToName");

		if (!Func) {
			Log("UKismetStringLibrary::Conv_StringToName: Failed to find function!");
			return FName();
		}

		struct KismetStringLibrary_Conv_StringToName final
		{
		public:
			FString InString;
			FName ReturnValue;
		};

		KismetStringLibrary_Conv_StringToName Parms{};

		Parms.InString = std::move(InString);

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		return Parms.ReturnValue;
	}
}