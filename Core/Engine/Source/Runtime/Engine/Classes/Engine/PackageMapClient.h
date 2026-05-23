#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"

class UNetConnection;
class UNetDriver;

class FNetGUIDCache {
public:
	bool SupportsObject(const UObject* Object, const TWeakObjectPtr<UObject>* WeakObjectPtr = nullptr) const;
};