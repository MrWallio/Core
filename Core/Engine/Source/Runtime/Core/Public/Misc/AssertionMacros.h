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
#define LIKELY(x)   __builtin_expect(!!(x), 1)
#else
#define UNLIKELY(x) (x)
#define LIKELY(x)   (x)
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

#define checkf(expr, format, ...)	{ if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__, TEXT(format), ##__VA_ARGS__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__, TEXT(format), ##__VA_ARGS__ ); CA_ASSUME(false); } }
#define checkfSlow(expr, format, ...)	checkf(expr, format, ##__VA_ARGS__)

#define verify(expr)	check(expr)
#define verifyf(expr, format, ...)	checkf(expr, format, ##__VA_ARGS__)
#define verifySlow(expr)	checkSlow(expr)

#define checkNoEntry()	check(!"Enclosing block should never be called")
#define unimplemented()	check(!"Unimplemented function called")
#define checkNoReentry()	{ static bool bBeenHere = false; checkf(!bBeenHere, TEXT("Enclosing block was called more than once")); bBeenHere = true; }
#define checkNoRecursion()	static int32 RecursionCounter = 0; checkf(RecursionCounter == 0, TEXT("Enclosing block was entered recursively")); const FRecursionScopeMarker ScopeMarker(RecursionCounter)

#define ensure(expr)	(LIKELY(!!(expr)) || (FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ), false))
#define ensureMsgf(expr, format, ...)	(LIKELY(!!(expr)) || (FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__, TEXT(format), ##__VA_ARGS__ ), false))
#define ensureAlways(expr)	ensure(expr)
#define ensureAlwaysMsgf(expr, format, ...)	ensureMsgf(expr, format, ##__VA_ARGS__)

struct FRecursionScopeMarker
{
    int32& Counter;
    FRecursionScopeMarker(int32& InCounter) : Counter(InCounter) { ++Counter; }
    ~FRecursionScopeMarker() { --Counter; }
};