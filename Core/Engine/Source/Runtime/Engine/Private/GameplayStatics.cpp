#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

AActor* UGameplayStatics::BeginDeferredActorSpawnFromClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner)
{
	if (Finder::FindUGameplayStatics_BeginDeferredActorSpawnFromClass()) {
		AActor* (*BeginDeferredActorSpawnFromClassInternal)(const UObject*, TSubclassOf<AActor>, const FTransform&, ESpawnActorCollisionHandlingMethod, AActor*) = decltype(BeginDeferredActorSpawnFromClassInternal)(ImageBase + Finder::FindUGameplayStatics_BeginDeferredActorSpawnFromClass());
		return BeginDeferredActorSpawnFromClassInternal(WorldContextObject, ActorClass, SpawnTransform, CollisionHandlingOverride, Owner);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.BeginDeferredActorSpawnFromClass");

		if (!Func)
			return nullptr;

		struct GameplayStatics_BeginDeferredActorSpawnFromClass final
		{
		public:
			const UObject* WorldContextObject;
			TSubclassOf<AActor> ActorClass;
			FTransform SpawnTransform;
			ESpawnActorCollisionHandlingMethod CollisionHandlingOverride;
			AActor* Owner;
			AActor* ReturnValue;
		};

		GameplayStatics_BeginDeferredActorSpawnFromClass Parms{};

		Parms.WorldContextObject = WorldContextObject;
		Parms.ActorClass = ActorClass;
		Parms.SpawnTransform = std::move(SpawnTransform);
		Parms.CollisionHandlingOverride = CollisionHandlingOverride;
		Parms.Owner = Owner;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		return Parms.ReturnValue;
	}
}

AActor* UGameplayStatics::FinishSpawningActor(AActor* Actor, const FTransform& SpawnTransform)
{
	// UE 4.25 Implementation
	if (Actor) {
		Actor->FinishSpawning(SpawnTransform);
	}

	return Actor;
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