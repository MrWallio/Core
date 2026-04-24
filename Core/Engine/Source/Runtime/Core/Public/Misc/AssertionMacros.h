#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

#if defined(_MSC_VER)
#define CA_ASSUME(expr) __assume(expr)
#else
#define CA_ASSUME(expr) ((void)0)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define UNLIKELY(x) (x)
#endif

static inline void _DebugBreakAndPromptForRemote()
{
#if _WIN32
    __debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
    __builtin_trap();
#else
    * (int*)0 = 0; // Force crash
#endif
}

struct FDebug
{
    static void AssertFailed(const char* Expr, const char* File, int32 Line, const TCHAR* Format = TEXT(""), ...);

    static void LogAssertFailedMessage(const char* Expr, const char* File, int32 Line, const TCHAR* Format = TEXT(""), ...);
};

#define check(expr)	{ if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(false); } }
#define checkSlow(expr)	{ if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed(#expr, __FILE__, __LINE__); CA_ASSUME(false); } }