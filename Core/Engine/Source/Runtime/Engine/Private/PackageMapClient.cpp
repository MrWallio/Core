#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/PackageMapClient.h"

bool FNetGUIDCache::SupportsObject(const UObject* Object, const TWeakObjectPtr<UObject>* WeakObjectPtr) const
{
	bool (*SupportsObjectInternal)(const FNetGUIDCache*, const UObject*, const TWeakObjectPtr<UObject>*) = decltype(SupportsObjectInternal)(ImageBase + Finder::FindFNetGUIDCache_SupportsObject());
	return SupportsObjectInternal(this, Object, WeakObjectPtr);
}