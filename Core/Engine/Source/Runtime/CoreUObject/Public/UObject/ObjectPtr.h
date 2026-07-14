#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

class UObject;

// UE5's TObjectPtr. In cooked/runtime builds (what this SDK targets) it is
// pointer-sized and behaves exactly like a raw T* -- the editor-only tracking
// members don't exist in shipping. This mirror models that cooked layout, so
// UE5 reflected object properties (stored as TObjectPtr<T>) read correctly.
// On 4.x, object properties are raw T* and this type simply isn't used.
template<typename T>
struct TObjectPtr
{
	T* Object;

	FORCEINLINE TObjectPtr() : Object(nullptr) {}
	FORCEINLINE TObjectPtr(T* InObject) : Object(InObject) {}
	FORCEINLINE TObjectPtr(std::nullptr_t) : Object(nullptr) {}

	FORCEINLINE T* Get() const { return Object; }

	FORCEINLINE operator T* () const { return Object; }
	FORCEINLINE T* operator->() const { return Object; }
	FORCEINLINE T& operator*() const { return *Object; }

	FORCEINLINE explicit operator bool() const { return Object != nullptr; }

	FORCEINLINE bool operator==(const TObjectPtr& Other) const { return Object == Other.Object; }
	FORCEINLINE bool operator!=(const TObjectPtr& Other) const { return Object != Other.Object; }
	FORCEINLINE bool operator==(const T* Other) const { return Object == Other; }
	FORCEINLINE bool operator!=(const T* Other) const { return Object != Other; }

	FORCEINLINE TObjectPtr& operator=(T* InObject) { Object = InObject; return *this; }
	FORCEINLINE TObjectPtr& operator=(std::nullptr_t) { Object = nullptr; return *this; }
};

static_assert(sizeof(TObjectPtr<UObject>) == 0x8, "TObjectPtr layout broke: cooked/runtime TObjectPtr is a single pointer (0x8 on x64)");
