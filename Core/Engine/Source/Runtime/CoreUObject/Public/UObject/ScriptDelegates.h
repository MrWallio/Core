#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Array.h"
#include "WeakObjectPtr.h"

class FScriptDelegate
{
public:
	FWeakObjectPtr Object;
	FName FunctionName;

public:
	FScriptDelegate()
		: Object(), FunctionName()
	{
	}

public:
	inline bool IsBound() const
	{
		return !FunctionName.IsNone() && Object.Get() != nullptr;
	}

	inline UObject* GetUObject() const
	{
		return Object.Get();
	}

	inline FName GetFunctionName() const
	{
		return FunctionName;
	}

	inline void BindUFunction(UObject* InObject, FName InFunctionName)
	{
		Object = FWeakObjectPtr(InObject);
		FunctionName = InFunctionName;
	}

	inline void Unbind()
	{
		Object.Reset();
		FunctionName = FName();
	}

	void ProcessDelegate(void* Parameters) const
	{
		UObject* BoundObject = Object.Get();
		if (!BoundObject || FunctionName.IsNone())
			return;

		UFunction* Function = BoundObject->FindFunction(FunctionName);
		if (!Function)
			return;

		BoundObject->ProcessEvent(Function, Parameters);
	}

public:
	inline bool operator==(const FScriptDelegate& Other) const
	{
		return Object.HasSameIndexAndSerialNumber(Other.Object) && FunctionName == Other.FunctionName;
	}

	inline bool operator!=(const FScriptDelegate& Other) const
	{
		return !(*this == Other);
	}
};

class FMulticastScriptDelegate
{
public:
	TArray<FScriptDelegate> InvocationList;

public:
	inline bool IsBound() const
	{
		return InvocationList.Num() > 0;
	}

	inline bool Contains(const FScriptDelegate& InDelegate) const
	{
		return InvocationList.Contains(InDelegate);
	}

	inline bool Contains(const UObject* InObject, FName InFunctionName) const
	{
		for (const FScriptDelegate& Delegate : InvocationList)
		{
			if (Delegate.GetFunctionName() == InFunctionName && Delegate.GetUObject() == InObject)
			{
				return true;
			}
		}

		return false;
	}

	inline void AddUnique(const FScriptDelegate& InDelegate)
	{
		InvocationList.AddUnique(InDelegate);
	}

	inline void Remove(const FScriptDelegate& InDelegate)
	{
		InvocationList.RemoveSingle(InDelegate);
	}

	inline void RemoveAll(const UObject* InObject)
	{
		for (int32 Index = InvocationList.Num() - 1; Index >= 0; --Index)
		{
			if (InvocationList[Index].GetUObject() == InObject)
			{
				InvocationList.RemoveAt(Index);
			}
		}
	}

	inline void Clear()
	{
		InvocationList.Empty();
	}

	void ProcessMulticastDelegate(void* Parameters) const
	{
		if (InvocationList.Num() == 0)
			return;

		TArray<FScriptDelegate> LocalCopy;
		LocalCopy.Append(InvocationList);

		for (const FScriptDelegate& Delegate : LocalCopy)
		{
			Delegate.ProcessDelegate(Parameters);
		}

		LocalCopy.Free();
	}
};

static_assert(sizeof(FScriptDelegate) == 0x10, "FScriptDelegate layout broke: UE 4.0-5.x expects FWeakObjectPtr + FName = 0x10");
static_assert(sizeof(FMulticastScriptDelegate) == 0x10, "FMulticastScriptDelegate layout broke: UE 4.0-5.x expects a single TArray = 0x10");
