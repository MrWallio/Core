#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

AActor* UGameplayStatics::BeginDeferredActorSpawnFromClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.BeginDeferredActorSpawnFromClass");

	if (!Func)
		return nullptr;

	return GetDefaultObj()->Call<AActor*>(Func, WorldContextObject, ActorClass, SpawnTransform, CollisionHandlingOverride, Owner);
}

AActor* UGameplayStatics::FinishSpawningActor(AActor* Actor, const FTransform& SpawnTransform)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.FinishSpawningActor");

	if (!Func)
		return nullptr;

	return GetDefaultObj()->Call<AActor*>(Func, Actor, SpawnTransform);
}

UObject* UGameplayStatics::SpawnObject(TSubclassOf<UObject> ObjectClass, UObject* Outer) {
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.SpawnObject");

	struct GameplayStatics_SpawnObject final
	{
	public:
		TSubclassOf<UObject> ObjectClass;
		UObject* Outer;
		UObject* ReturnValue;
	};

	GameplayStatics_SpawnObject Parms{};

	Parms.ObjectClass = ObjectClass;
	Parms.Outer = Outer;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

float UGameplayStatics::GetTimeSeconds(const class UObject* WorldContextObject)
{
	static UFunction* Func = nullptr;
	static UObject* DefaultObj = GetDefaultObj();

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.GetTimeSeconds");

	struct GameplayStatics_GetTimeSeconds final
	{
	public:
		const UObject* WorldContextObject;
		float ReturnValue;
	};

	GameplayStatics_GetTimeSeconds Parms{};

	Parms.WorldContextObject = WorldContextObject;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

void UGameplayStatics::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>* OutActors)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.GetAllActorsOfClass");

	struct GameplayStatics_GetAllActorsOfClass
	{
	public:
		const UObject* WorldContextObject;
		TSubclassOf<AActor> ActorClass;
		TArray<AActor*> OutActors;
	};

	GameplayStatics_GetAllActorsOfClass Parms{};

	Parms.WorldContextObject = WorldContextObject;
	Parms.ActorClass = ActorClass;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	if (OutActors != nullptr)
		*OutActors = std::move(Parms.OutActors);
}

APlayerController* UGameplayStatics::GetPlayerController(const UObject* WorldContextObject, int32 PlayerIndex)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.GetPlayerController");

	struct GameplayStatics_GetPlayerController
	{
	public:
		const UObject* WorldContextObject;
		int32 PlayerIndex;
		uint8 Pad_C[0x4];
		APlayerController* ReturnValue;
	};

	GameplayStatics_GetPlayerController Parms{};

	Parms.WorldContextObject = WorldContextObject;
	Parms.PlayerIndex = PlayerIndex;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}