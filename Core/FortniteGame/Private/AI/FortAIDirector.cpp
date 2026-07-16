#include "pch.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/BuildingActor/BuildingActor.h"

void AFortAIDirector::Activate()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("Activate");

	Call(Func);
}

AFortAIDirector* AFortAIDirector::GetCurrent(UObject* WorldContextObject) {
	//Log("AFortAIDirector::GetCurrent");
	if (!WorldContextObject) {
		Log("AFortAIDirector::GetCurrent: Failed due to no WorldContextObject!");
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		Log("AFortAIDirector::GetCurrent: Failed due to no World!");
		return nullptr;
	}

	AFortGameModeZone* GameMode = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!GameMode) {
		Log("AFortAIDirector::GetCurrent: Failed because GameMode is either null or not an AFortGameModeZone!");
		return nullptr;
	}

	return GameMode->AIDirector;
}

void AFortAIDirector::Hook() {
	TArray<uintptr_t> GetCurrentPatchCallSites;

	{
		uintptr_t Addr = 0;

		uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Could not create encounter sequence with given tags: %s, generated sequence not found").Get();
		if (StringAddr) {
			int Skipped = 0;
			for (int i = 0; i < 1024; i++)
			{
				auto Ptr = (uint8_t*)(StringAddr - i);
				if (*Ptr == 0xE8)
				{
					if (Skipped == 2) {
						Addr = uint64_t(Ptr);
						break;
					}
					Skipped++;
				}
			}
		}
		else {
			Log("AFortAIDirector::Hook: string ref for encounter-sequence patch not found");
		}

		GetCurrentPatchCallSites.Add(Addr);
	}

	{
		uintptr_t Addr = 0;

		uintptr_t HandleDamagedVFT = Finder::FindABuildingActor_HandleDamagedVFT();
		if (HandleDamagedVFT) {
			uintptr_t HandleDamagedAddr = (uintptr_t)ABuildingActor::StaticClass()->GetDefaultObject()->VTable[HandleDamagedVFT];
			uintptr_t FunctionEnd = Memcury::Scanner(HandleDamagedAddr).ScanFor({ 0x5D, 0xC3 }).Get();

			for (int i = 0; i < 1024; i++)
			{
				auto Cursor = (FunctionEnd - i);
				if (*(uint8*)(Cursor) == 0xFF && *(uint8*)(Cursor + 1) == 0x90 && *(uint8*)(Cursor + 9) == 0xE8)
				{
					Addr = Cursor + 9;
					break;
				} 
				else if (*(uint8*)(Cursor) == 0xFF && *(uint8*)(Cursor + 1) == 0x90 && *(uint8*)(Cursor + 9) == 0x38)
				{
					Addr = Cursor + 9;
					break;
				}
			}
		}

		GetCurrentPatchCallSites.Add(Addr);
	}

	for (int32 i = 0; i < GetCurrentPatchCallSites.Num(); i++) {
		uintptr_t Patch = GetCurrentPatchCallSites[i];
		if (Patch) {
			Log("AFortAIDirector::GetCurrent Patch: 0x" + std::format("{:X}", (Patch - ImageBase)));
			PatchCallFar(Patch, GetCurrent);
		}
		else {
			Log("Failed to find patch for AFortAIDirector::GetCurrent: Index (" + std::to_string(i) + ")");
		}
	}

	Log("AFortAIDirector Hooked");
}