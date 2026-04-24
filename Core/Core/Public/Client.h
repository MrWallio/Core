#pragma once
#include "pch.h"

#include "Configuration.h"

class Client {
public:
	static void Init(FCoreConfig& Config);
private:
	static void InitConsole(FCoreConfig& Config);

	static DWORD ClientThread(LPVOID);
};