#pragma once
#include "pch.h"

class Patches {
public:
	static void ApplyPatches() {
		{
			uintptr_t Patch1 = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 F6 48 8B F9 89 B1 ? ? ? ? 48 8B 89 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 89 B7").Get();
			if (Patch1) {
				MH_CreateHook((LPVOID)Patch1, RetNullptr, nullptr);
				Log("Patched: " + std::to_string(Patch1 - ImageBase) + " with RetNullptr");
			}
		}
	}
};