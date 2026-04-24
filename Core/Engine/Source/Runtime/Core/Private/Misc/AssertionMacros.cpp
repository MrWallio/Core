#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"

void FDebug::AssertFailed(const char* Expr, const char* File, int32 Line, const TCHAR* Format, ...)
{
    void (*AssertFailedInternal)(const char* Expr, const char* File, int32 Line, const TCHAR * Format, va_list Args)
        = decltype(AssertFailedInternal)(ImageBase + Finder::FindFDebug_AssertFailed());

    va_list args;
    va_start(args, Format);

    AssertFailedInternal(Expr, File, Line, Format, args);

    va_end(args);
}

void FDebug::LogAssertFailedMessage(const char* Expr, const char* File, int32 Line, const TCHAR* Format, ...)
{
    std::string msg = std::string("ASSERT: ") + Expr + " at " + File + ":" + std::to_string(Line);

    if (Format && Format[0] != L'\0')
    {
        va_list args;
        va_start(args, Format);

        wchar_t wbuf[2048];
        vswprintf(wbuf, 2048, Format, args);

        char buf[2048];
        size_t converted = 0;
        wcstombs_s(&converted, buf, sizeof(buf), wbuf, _TRUNCATE);

        msg += std::string(" - ") + buf;

        va_end(args);
    }

    Log(msg);
}