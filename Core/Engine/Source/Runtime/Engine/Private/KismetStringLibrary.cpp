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

FString UKismetStringLibrary::ToUpper(const FString& SourceString)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.ToUpper");

	struct KismetStringLibrary_ToUpper
	{
	public:
		FString SourceString;
		FString ReturnValue;
	};

	KismetStringLibrary_ToUpper Parms{};

	Parms.SourceString = std::move(SourceString);

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

bool UKismetStringLibrary::Contains(const class FString& SearchIn, const class FString& Substring, bool bUseCase, bool bSearchFromEnd)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.Contains");

	struct KismetStringLibrary_Contains
	{
	public:
		FString SearchIn;
		FString Substring;
		bool bUseCase;
		bool bSearchFromEnd;
		bool ReturnValue;
	};

	KismetStringLibrary_Contains Parms{};

	Parms.SearchIn = std::move(SearchIn);
	Parms.Substring = std::move(Substring);
	Parms.bUseCase = bUseCase;
	Parms.bSearchFromEnd = bSearchFromEnd;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UKismetStringLibrary::FindSubstring(const class FString& SearchIn, const class FString& Substring, bool bUseCase, bool bSearchFromEnd, int32 StartPosition)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.FindSubstring");

	struct KismetStringLibrary_FindSubstring
	{
	public:
		FString SearchIn;
		FString Substring;
		bool bUseCase;
		bool bSearchFromEnd;
		uint8 Pad_22[0x2];
		int32 StartPosition;
		int32 ReturnValue;
		uint8 Pad_2C[0x4];
	};

	KismetStringLibrary_FindSubstring Parms{};

	Parms.SearchIn = std::move(SearchIn);
	Parms.Substring = std::move(Substring);
	Parms.bUseCase = bUseCase;
	Parms.bSearchFromEnd = bSearchFromEnd;
	Parms.StartPosition = StartPosition;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FString UKismetStringLibrary::Left(const class FString& SourceString, int32 Count)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.Left");

	struct KismetStringLibrary_Left
	{
	public:
		FString SourceString;
		int32 Count;
		uint8 Pad_14[0x4];
		FString ReturnValue;
	};

	KismetStringLibrary_Left Parms{};

	Parms.SourceString = std::move(SourceString);
	Parms.Count = Count;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}