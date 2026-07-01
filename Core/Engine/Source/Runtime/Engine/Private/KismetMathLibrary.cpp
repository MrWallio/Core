#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

int32 UKismetMathLibrary::RandomIntegerInRange(int32 Min, int32 Max)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.RandomIntegerInRange");

	if (!Func) {
		Log("UKismetMathLibrary::RandomIntegerInRange: Failed to find function!");
		return 0;
	}

	struct KismetMathLibrary_RandomIntegerInRange final
	{
	public:
		int32 Min;
		int32 Max;
		int32 ReturnValue;
	};

	KismetMathLibrary_RandomIntegerInRange Parms{};

	Parms.Min = Min;
	Parms.Max = Max;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

float UKismetMathLibrary::RandomFloatInRange(float Min, float Max)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.RandomFloatInRange");

	if (!Func) {
		Log("UKismetMathLibrary::RandomFloatInRange: Failed to find function!");
		return 0.0f;
	}

	struct KismetMathLibrary_RandomFloatInRange final
	{
	public:
		float Min;
		float Max;
		float ReturnValue;
	};

	KismetMathLibrary_RandomFloatInRange Parms{};

	Parms.Min = Min;
	Parms.Max = Max;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UKismetMathLibrary::Min(int32 A, int32 B)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Min");

	if (!Func) {
		Log("UKismetMathLibrary::Min: Failed to find function!");
		return 0;
	}

	struct KismetMathLibrary_Min final
	{
	public:
		int32 A;
		int32 B;
		int32 ReturnValue;
	};

	KismetMathLibrary_Min Parms{};

	Parms.A = A;
	Parms.B = B;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UKismetMathLibrary::FTrunc(float A)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.FTrunc");

	if (!Func) {
		Log("UKismetMathLibrary::FTrunc: Failed to find function!");
		return 0;
	}

	struct KismetMathLibrary_FTrunc final
	{
	public:
		float A;
		int32 ReturnValue;
	};

	KismetMathLibrary_FTrunc Parms{};

	Parms.A = A;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UKismetMathLibrary::Max(int32 A, int32 B)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Max");

	if (!Func) {
		Log("UKismetMathLibrary::Max: Failed to find function!");
		return 0;
	}

	struct KismetMathLibrary_Max final
	{
	public:
		int32 A;
		int32 B;
		int32 ReturnValue;
	};

	KismetMathLibrary_Max Parms{};

	Parms.A = A;
	Parms.B = B;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UKismetMathLibrary::Clamp(int32 Value, int32 Min, int32 Max)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Clamp");

	if (!Func) {
		Log("UKismetMathLibrary::Clamp: Failed to find function!");
		return 0;
	}

	struct KismetMathLibrary_Clamp final
	{
	public:
		int32 Value;
		int32 Min;
		int32 Max;
		int32 ReturnValue;
	};

	KismetMathLibrary_Clamp Parms{};

	Parms.Value = Value;
	Parms.Min = Min;
	Parms.Max = Max;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

float UKismetMathLibrary::Lerp(float A, float B, float Alpha)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Lerp");

	if (!Func) {
		Log("UKismetMathLibrary::Lerp: Failed to find function!");
		return 0.0f;
	}

	struct KismetMathLibrary_Lerp final
	{
	public:
		float A;
		float B;
		float Alpha;
		float ReturnValue;
	};

	KismetMathLibrary_Lerp Parms{};

	Parms.A = A;
	Parms.B = B;
	Parms.Alpha = Alpha;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UKismetMathLibrary::Round(float A)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Round");

	if (!Func) {
		Log("UKismetMathLibrary::Round: Failed to find function!");
		return 0;
	}

	struct KismetMathLibrary_Round final
	{
	public:
		float A;
		int32 ReturnValue;
	};

	KismetMathLibrary_Round Parms{};

	Parms.A = A;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

float UKismetMathLibrary::RandomFloat()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.RandomFloat");

	if (!Func) {
		Log("UKismetMathLibrary::RandomFloat: Failed to find function!");
		return 0.0f;
	}

	struct KismetMathLibrary_RandomFloat final
	{
	public:
		float ReturnValue;
	};

	KismetMathLibrary_RandomFloat Parms{};

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FVector UKismetMathLibrary::GetForwardVector(const FRotator& InRot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.GetForwardVector");

	if (!Func) {
		return FVector(0, 0, 0);
	}

	return GetDefaultObj()->Call<FVector>(Func, InRot);
}

FVector UKismetMathLibrary::GetRightVector(const FRotator& InRot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.GetRightVector");

	if (!Func) {
		return FVector(0, 0, 0);
	}

	return GetDefaultObj()->Call<FVector>(Func, InRot);
}

FVector UKismetMathLibrary::GetUpVector(const FRotator& InRot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.GetUpVector");

	if (!Func) {
		return FVector(0, 0, 0);
	}

	return GetDefaultObj()->Call<FVector>(Func, InRot);
}