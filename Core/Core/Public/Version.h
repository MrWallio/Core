#pragma once
#include "pch.h"

class Version {
public:
	static std::string VersionString;
	static double Engine_Version;
	static double Fortnite_Version;
	static int Fortnite_CL;

public:
	static void SetupVersion();
};