#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "SoftObjectPath.h"
#include "WeakObjectPtr.h"

template <typename TObjectID>
class TPersistentObjectPtr
{
public:
    FWeakObjectPtr WeakPtr;
    int32 TagAtLastTest;
    TObjectID ObjectID;

public:
    const UObject* Get() const
    {
        return WeakPtr.Get();
    }
    const UObject* operator->() const
    {
        return WeakPtr.Get();
    }
};