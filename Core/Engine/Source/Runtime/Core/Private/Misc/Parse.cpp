#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Parse.h"

bool FParse::Param(const TCHAR* Stream, const TCHAR* Param)
{
	bool (*ParamInternal)(const TCHAR*, const TCHAR*) = decltype(ParamInternal)(ImageBase + Finder::FindFParse_Param());
	return ParamInternal(Stream, Param);
}