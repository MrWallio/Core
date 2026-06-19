#pragma once
#include "pch.h"

#include "Configuration.h"

class Client {
public:
	static void Init(FCoreConfig& Config);
private:
	static DWORD ClientThread(LPVOID);
};