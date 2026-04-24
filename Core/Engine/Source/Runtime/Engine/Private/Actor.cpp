#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

ENetMode AActor::InternalGetNetMode(AActor* This)
{
	return InternalGetNetModeOG(This);
	return NM_DedicatedServer;
}

void AActor::FinishSpawning(const FTransform& UserTransform, bool bIsDefaultTransform, const FComponentInstanceDataCache* InstanceDataCache)
{
	void (*FinishSpawningInternal)(AActor*, const FTransform&, bool, const FComponentInstanceDataCache*) = decltype(FinishSpawningInternal)(ImageBase + Finder::FindAActor_FinishSpawning());
	FinishSpawningInternal(this, UserTransform, bIsDefaultTransform, InstanceDataCache);
}