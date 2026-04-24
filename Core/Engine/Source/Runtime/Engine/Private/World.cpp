#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

UWorld* UWorld::GetWorld() {
	if (ServerOffsets::GWorld != 0)
		return *reinterpret_cast<UWorld**>(ImageBase + ServerOffsets::GWorld);

	if (UEngine* Engine = UEngine::GetEngine())
	{
		if (!Engine->GameViewport)
			return nullptr;

		return Engine->GameViewport->World;
	}

	return nullptr;
}

ENetMode UWorld::InternalGetNetMode(UWorld* This)
{
	return NM_DedicatedServer;
}

AActor* UWorld::SpawnActor(UClass* Class, FVector Location, FRotator Rotation, AActor* Owner) {
	FTransform Transform;

	Transform.Translation = Location;
	Transform.Rotation = FQuat(Rotation);
	Transform.Scale3D.X = 1.f;
	Transform.Scale3D.Y = 1.f;
	Transform.Scale3D.Z = 1.f;

	return SpawnActor(Class, Transform, Owner);
}

AActor* UWorld::SpawnActor(UClass* Class, FTransform Transform, AActor* Owner) {
	auto Actor = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), Class, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, Owner);
	if (Actor) {
		return UGameplayStatics::FinishSpawningActor(Actor, Transform);
	}
	else {
		return nullptr;
	}
}

bool UWorld::ServerTravel(const FString& FURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	bool (*ServerTravelInternal)(UWorld*, const FString&, bool, bool) = decltype(ServerTravelInternal)(ImageBase + Finder::FindUWorld_ServerTravel());
	return ServerTravelInternal(this, FURL, bAbsolute, bShouldSkipGameNotify);
}