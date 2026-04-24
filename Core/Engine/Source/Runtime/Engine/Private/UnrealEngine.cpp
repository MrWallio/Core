#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

void FWorldContext::SetCurrentWorld(UWorld* World)
{
	void (*SetCurrentWorldInternal)(FWorldContext*, UWorld*) = decltype(SetCurrentWorldInternal)(ImageBase + Finder::FindFWorldContext_SetCurrentWorld());
	SetCurrentWorldInternal(this, World);
}

UEngine* UEngine::GetEngine() {
	if (ServerOffsets::GEngine != 0)
		return *reinterpret_cast<UEngine**>(ImageBase + ServerOffsets::GEngine);

	static UEngine* GEngine = nullptr;

	if (GEngine)
		return GEngine;

	for (int i = 0; i < FUObjectArray::Num(); i++)
	{
		FUObjectItem* Item = FUObjectArray::IndexToObject(i);
		if (!Item)
			continue;

		UObjectBase* Object = Item->Object;

		if (!Object)
			continue;

		if (!Object->IsA(UEngine::StaticClass())) {
			continue;
		}

		if (Object->IsDefaultObject()) {
			continue;
		}

		GEngine = (UEngine*)Object;
		break;
	}

	if (!GEngine || !GEngine->IsA(UEngine::StaticClass())) {
		Log("UEngine::GetEngine failed to find GEngine!");
		return GEngine = nullptr;
	}

	return GEngine;
}