#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "MemoryBase.h"
#include "Platform.h"

struct FMemory
{
	static void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static void Free(void* Original);
};