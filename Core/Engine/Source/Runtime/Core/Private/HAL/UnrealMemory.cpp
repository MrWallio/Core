#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"

void* FMemory::Malloc(SIZE_T Count, uint32 Alignment) {
	return Realloc(nullptr, Count, Alignment);
}

void* FMemory::Realloc(void* Ptr, SIZE_T NewSize, uint32 Alignment)
{
	static void* (*ReallocFunc)(void*, SIZE_T, uint32) = decltype(ReallocFunc)(ImageBase + Finder::FindFMemory_Realloc());
	return ReallocFunc(Ptr, NewSize, Alignment);
}

void FMemory::Free(void* Original)
{
	static void (*FreeFunc)(void*) = decltype(FreeFunc)(ImageBase + Finder::FindFMemory_Free());
	FreeFunc(Original);
}