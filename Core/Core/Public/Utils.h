#pragma once
#include "pch.h"

#include "Configuration.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"

class Utils {
public:
	static void InitConsole(FCoreConfig& Config);

	static uintptr_t ResolveRipRelative(uintptr_t instr, int dispOffset, int instrLen);

	static void DumpGObjects(bool bLog = false);
	static void DumpClassProperties(const char* ClassName);

	static void Hook();

	static FString GetDefaultMapName();

	static FString BuildTravelURL(FString MapPath, std::vector<std::pair<std::string, std::string>> Options = {});

	static void SetLogVerbosity();

	static void RemoveLocalPlayer();
	static void LoadWorld();
};