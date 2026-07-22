#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"

void* FMemory::Malloc(SIZE_T Count, uint32 Alignment) {
	return Realloc(nullptr, Count, Alignment);
}

void* FMemory::Realloc(void* Ptr, SIZE_T NewSize, uint32 Alignment)
{
	static uintptr_t ReallocOffset = Finder::FindFMemory_Realloc();
	if (!ReallocOffset)
		return nullptr;

	static void* (*ReallocFunc)(void*, SIZE_T, uint32) = decltype(ReallocFunc)(ImageBase + ReallocOffset);
	return ReallocFunc(Ptr, NewSize, Alignment);
}

void FMemory::Free(void* Original)
{
	static uintptr_t FreeOffset = Finder::FindFMemory_Free();
	if (!FreeOffset)
		return;

	static void (*FreeFunc)(void*) = decltype(FreeFunc)(ImageBase + FreeOffset);
	FreeFunc(Original);
}