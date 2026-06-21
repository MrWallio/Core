#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogVerbosity.h"

class FName;

using FLogCategoryName = FName;

struct FMsg
{
public:
    static void Logf_InternalImpl(const char* File, int32 Line, const FLogCategoryName& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, va_list a);

    template <typename... Types>
    static void Logf_Internal(const char* File, int32 Line, const FLogCategoryName& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, ...)
    {
        va_list list;
        va_start(list, Fmt);
        Logf_InternalImpl(File, Line, Category, Verbosity, Fmt, list);
        va_end(list);
    }
};

#define UE_LOG(Category, Verbosity, Format, ...) \
{ \
    FMsg::Logf_Internal(0, 0, Category, ELogVerbosity::Verbosity, Format, ##__VA_ARGS__); \
}