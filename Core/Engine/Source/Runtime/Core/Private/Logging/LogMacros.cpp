#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"

void FMsg::Logf_InternalImpl(const char* File, int32 Line, const FLogCategoryName& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, va_list a)
{
    static void (*Logf_InternalImplOG)(const char* File, int32 Line, const FLogCategoryName & Category, ELogVerbosity::Type Verbosity, const TCHAR * Fmt, va_list a) = decltype(Logf_InternalImplOG)(ImageBase + Finder::FindFMsg_Logf());
    Logf_InternalImplOG(File, Line, Category, Verbosity, Fmt, a);
}