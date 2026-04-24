#include "pch.h"
#include "../Public/Configuration.h"

#include "Engine/Source/Runtime/Core/Public/Misc/Parse.h"

FCoreConfig ConfigurationManager::Config;

void ConfigurationManager::LoadConfig() {
	FCoreConfig NewConfig = FCoreConfig();

	NewConfig.bEnableClientConsole = FParse::Param(GetCommandLineW(), TEXT("bEnableClientConsole"));
	NewConfig.bIsClient = FParse::Param(GetCommandLineW(), TEXT("bIsClient"));
	NewConfig.bSetClientLogVerbosity = FParse::Param(GetCommandLineW(), TEXT("bSetClientLogVerbosity"));

	Log("Configuration Loaded:");
	Log("bEnableClientConsole: " + std::to_string(NewConfig.bEnableClientConsole));
	Log("bIsClient: " + std::to_string(NewConfig.bIsClient));
	Log("bSetClientLogVerbosity: " + std::to_string(NewConfig.bSetClientLogVerbosity));

	Config = NewConfig;
}

FCoreConfig& ConfigurationManager::GetConfig() {
	return Config;
}