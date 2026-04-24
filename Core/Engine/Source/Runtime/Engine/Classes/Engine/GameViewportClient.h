#pragma once
#include "pch.h"

#include "ScriptViewportClient.h"

class UWorld;
class UConsole;

class UGameViewportClient : public UScriptViewportClient {
public:
	DefineUnrealClass(UGameViewportClient);
public:
	DefineUProperty(UWorld*, World);

	DefineUProperty(UConsole*, ViewportConsole);
};