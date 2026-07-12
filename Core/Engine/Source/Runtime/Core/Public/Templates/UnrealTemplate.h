#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T&> { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

template <typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	return (typename TRemoveReference<T>::Type&&)Obj;
}

template <typename T>
FORCEINLINE T&& MoveTempIfPossible(T&& Obj)
{
	return (T&&)Obj;
}

template <typename T>
FORCEINLINE T CopyTemp(const T& Val)
{
	return Val;
}

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}

template <typename T>
inline void Swap(T& A, T& B)
{
	T Temp = MoveTemp(A);
	A = MoveTemp(B);
	B = MoveTemp(Temp);
}

template <typename T, typename ArgType = T>
T Exchange(T& Value, ArgType&& NewValue)
{
	T OldValue = MoveTemp(Value);
	Value = Forward<ArgType>(NewValue);
	return OldValue;
}
