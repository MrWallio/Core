// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <map>
#include <format>
#include <print>
#include <array>
#include <minmax.h>

#include "../includes/memcury.h"
#include "../includes/MinHook/MinHook.h"

#include "Core/Public/Offsets.h"
#include "Core/Public/Configuration.h"

inline uintptr_t ImageBase = (uintptr_t)GetModuleHandleA(0);

#ifndef INDEX_NONE
#define INDEX_NONE -1
#endif

inline void Log(const std::string& msg)
{
	static bool firstCall = true;

	FCoreConfig& Config = ConfigurationManager::GetConfig();
	std::string FileName = Config.bIsClient ? "Client_log.txt" : "Server_log.txt";
	std::string LogType = Config.bIsClient ? "Client" : "Server";

	if (firstCall)
	{
		std::ofstream logFile(FileName, std::ios::trunc);
		if (logFile.is_open())
		{
			logFile << "Log" + LogType + ": Log file initialized!\n";
			logFile.close();
		}
		firstCall = false;
	}

	std::ofstream logFile(FileName, std::ios::app);
	if (logFile.is_open())
	{
		logFile << "Log" + LogType + ": " << msg << std::endl;
		logFile.close();
	}

	std::cout << "Log" + LogType + ": " << msg << std::endl;
}

#endif //PCH_H
