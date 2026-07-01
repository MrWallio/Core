#pragma once
#include "pch.h"

#include "Version.h"

class Patches {
public:
	static void ApplyPatches() {
		// i like to keep patches at a minimal and if i can i would like to figure out a way to not use these patches

		if (Version::Fortnite_Version >= 1.63 && Version::Fortnite_Version <= 3.5) {
			uintptr_t Patch1 = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 33 F6 48 8B F9 89 B1 ? ? ? ? 48 8B 89 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 89 B7").Get();
			if (Patch1) {
				MH_CreateHook((LPVOID)Patch1, RetNullptr, nullptr);
				Log("Patched: " + std::to_string(Patch1 - ImageBase) + " with RetNullptr");
			}

			uintptr_t Patch2 = Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B ? 49 89 73 ? 49 89 7B ? 55 41 54 41 55 41 56 41 57 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 45 0F 29 A3").Get();
			if (Patch2) {
				MH_CreateHook((LPVOID)Patch2, RetNullptr, nullptr);
				Log("Patched: " + std::to_string(Patch2 - ImageBase) + " with RetNullptr");
			}

			uintptr_t Patch3 = Memcury::Scanner::FindPattern("40 53 55 56 48 81 EC ? ? ? ? 33 F6").Get();
			if (Patch3) {
				MH_CreateHook((LPVOID)Patch3, True, nullptr);
				Log("Patched: " + std::to_string(Patch3 - ImageBase) + " with True");
			}
		}

		if (Version::Fortnite_Version >= 2.1 && Version::Fortnite_Version <= 3.5)
		{
			uintptr_t WidgetCrashPatch = Memcury::Scanner::FindStringRef(L"Unable to create the widget {0}, no outer provided.").FindFunctionStart().Get();
			
			if (WidgetCrashPatch) {
				MH_CreateHook(LPVOID(WidgetCrashPatch), RetNullptr, 0);
				Log("Patched: " + std::to_string(WidgetCrashPatch - ImageBase) + " with RetNullptr");
			}

			uintptr_t Patch4 = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 3D ? ? ? ? ? 49 89 5B ? 48 8B 1D").Get();
			if (Patch4) {
				MH_CreateHook((LPVOID)Patch4, RetNullptr, nullptr);
				Log("Patched: " + std::to_string(Patch4 - ImageBase) + " with RetNullptr");
			}
		}

		if (Version::Fortnite_Version >= 3.0 && Version::Fortnite_Version <= 3.5) {
			uintptr_t Patch6 = Memcury::Scanner::FindPattern("40 53 55 56 48 81 EC ? ? ? ? 33 F6 48 8B D9").Get();
			if (Patch6) {
				MH_CreateHook((LPVOID)Patch6, RetNull, nullptr);
				Log("Patched: " + std::to_string(Patch6 - ImageBase) + " with RetNull");
			}
		}

		/*if (Version::Fortnite_Version == 3.3) { // replgraph very unfinished lol
			uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"UFortReplicationGraph is disabled").Get();
			if (StringAddr) {
				for (int i = 0; i < 256; i++) {
					auto Ptr = (uint8_t*)(StringAddr - i);
					if (*Ptr == 0x75) {
						PatchByte((uintptr_t)Ptr, 0xEB);
						Log("Patched: " + std::to_string((uintptr_t)Ptr - ImageBase) + " with 0xEB");
						break;
					}
				}
			}
		}*/

		if (Version::Fortnite_Version >= 3.5 && Version::Fortnite_Version <= 3.6) {
			uintptr_t Patch7 = Memcury::Scanner::FindPattern("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 89 5B ? 48 8D 05 ? ? ? ? 48 8B 1D").Get();
			if (Patch7) {
				MH_CreateHook((LPVOID)Patch7, RetNullptr, nullptr);
				Log("Patched: " + std::to_string(Patch7 - ImageBase) + " with RetNullptr");
			}

			uintptr_t Patch8 = Memcury::Scanner::FindPattern("48 8B C4 57 48 81 EC ? ? ? ? 4C 8B 82").Get();
			if (Patch8) {
				MH_CreateHook((LPVOID)Patch8, True, nullptr);
				Log("Patched: " + std::to_string(Patch8 - ImageBase) + " with True"); // bool AFortPoiVolume::OverlapsPawn
			}

			uintptr_t Patch9 = Memcury::Scanner::FindPattern("4C 8B DC 48 81 EC ? ? ? ? 49 89 5B ? 48 8D 05 ? ? ? ? 49 89 6B ? 48 8B D9 49 89 73 ? 33 F6").Get();
			if (Patch9) {
				MH_CreateHook((LPVOID)Patch9, RetNull, nullptr);
				Log("Patched: " + std::to_string(Patch9 - ImageBase) + " with RetNull");
			}
		}
	}
};