#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

inline uint32 HashCombine(uint32 A, uint32 C)
{
	uint32 B = 0x9e3779b9;
	A += B;

	A -= C;	A ^= (C >> 13);
	B -= C;	B ^= (A << 8);
	C -= A;	C ^= (B >> 13);

	A -= B;	A ^= (C >> 12);
	B -= C;	B ^= (A << 16);
	C -= A;	C ^= (B >> 5);

	A -= B;	A ^= (C >> 3);
	B -= C;	B ^= (A << 10);
	C -= A;	C ^= (B >> 15);

	return C;
}

FORCEINLINE uint32 GetTypeHash(const uint8 A) { return A; }
FORCEINLINE uint32 GetTypeHash(const int8 A) { return (uint32)A; }
FORCEINLINE uint32 GetTypeHash(const uint16 A) { return A; }
FORCEINLINE uint32 GetTypeHash(const int16 A) { return (uint32)A; }
FORCEINLINE uint32 GetTypeHash(const int32 A) { return (uint32)A; }
FORCEINLINE uint32 GetTypeHash(const uint32 A) { return A; }
FORCEINLINE uint32 GetTypeHash(const wchar_t A) { return (uint32)A; }

FORCEINLINE uint32 GetTypeHash(const uint64 A) { return (uint32)A + ((uint32)(A >> 32) * 23); }
FORCEINLINE uint32 GetTypeHash(const int64 A) { return GetTypeHash((uint64)A); }

FORCEINLINE uint32 GetTypeHash(const float Value) { return *(const uint32*)&Value; }
FORCEINLINE uint32 GetTypeHash(const double Value) { return GetTypeHash(*(const uint64*)&Value); }

inline uint32 PointerHash(const void* Key, uint32 C = 0)
{
	const uint64 PtrInt = (uint64)(uintptr_t)Key >> 4;

	if (Version::Engine_Version >= 5.0)
	{
		return GetTypeHash(PtrInt);
	}

	return HashCombine((uint32)PtrInt, C);
}

template<typename T>
FORCEINLINE uint32 GetTypeHash(T* Ptr)
{
	return PointerHash((const void*)Ptr);
}

template<typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
FORCEINLINE uint32 GetTypeHash(EnumType E)
{
	return GetTypeHash((std::underlying_type_t<EnumType>)E);
}
