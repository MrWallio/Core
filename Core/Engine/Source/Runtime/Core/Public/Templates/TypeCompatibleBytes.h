#pragma once
#include "pch.h"

template<int32_t Size, uint32_t Alignment>
struct TAlignedBytes
{
	alignas(Alignment) uint8_t Pad[Size];
};