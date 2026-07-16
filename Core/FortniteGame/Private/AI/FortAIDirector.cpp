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

void AFortAIDirector::Hook()
{
	{
		uintptr_t GetCurrentStub = 0;
		uint32_t GetWorldDisp = 0;

		uintptr_t StringAddr = Memcury::Scanner::FindStringRef(L"Could not create encounter sequence with given tags: %s, generated sequence not found").Get();
		if (StringAddr)
		{
			uintptr_t FuncStart = Memcury::Scanner(StringAddr).FindFunctionStart().Get();
			uintptr_t FuncEnd = Memcury::Scanner(StringAddr).FindFunctionEnd().Get();

			for (uintptr_t p = FuncStart; p && p + 5 <= FuncEnd; p++)
			{
				if (*reinterpret_cast<const uint8_t*>(p) != 0xE8)
					continue;

				uintptr_t Target = Memcury::PE::Address(p).RelativeOffset(1).Get();
				if (!IsReturnNullStub(Target))
					continue;

				GetCurrentStub = Target;
				FindIndirectCallBefore(p, 0, 16, GetWorldDisp);
				break;
			}
		}

		if (!GetCurrentStub || !GetWorldDisp)
		{
			Log("AFortAIDirector::Hook: failed to resolve GetCurrent stub / GetWorld offset; aborting.");
			return;
		}

		int32 DirectorSize = AFortAIDirector::GetSize();

		std::vector<uintptr_t> CallSites;
		for (uintptr_t CallSite : FindCallRefsToAddress(GetCurrentStub))
			if (CallArgFromAccessor(CallSite, GetWorldDisp, 28) || (DirectorSize > 0 && CallResultIsSizedObject(CallSite, 0x400, DirectorSize)))
				CallSites.push_back(CallSite);

		for (uintptr_t CallSite : CallSites)
			PatchCallFar(CallSite, GetCurrent);

		Log("AFortAIDirector Hooked (" + std::to_string(CallSites.size()) + " GetCurrent callsites, stub 0x" + std::format("{:X}", (GetCurrentStub - ImageBase)) + ", GetWorld vtable +0x" + std::format("{:X}", GetWorldDisp) + ", director size 0x" + std::format("{:X}", DirectorSize) + ")");
	}

	Log("AFortAIDirector Hooked");
}