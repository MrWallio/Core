#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

struct FParse {
	/** Checks if a command-line parameter exists in the stream. */
	static bool Param(const TCHAR* Stream, const TCHAR* Param);
};