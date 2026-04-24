#pragma once
#include "pch.h"

inline bool* GIsClient = (bool*)(ImageBase + ServerOffsets::GIsClient);
inline bool* GIsServer = (bool*)(ImageBase + ServerOffsets::GIsServer);

namespace CoreGlobals {
	inline void Init() {
		GIsClient = reinterpret_cast<bool*>(ImageBase + ServerOffsets::GIsClient);
		GIsServer = reinterpret_cast<bool*>(ImageBase + ServerOffsets::GIsServer);
	}
}