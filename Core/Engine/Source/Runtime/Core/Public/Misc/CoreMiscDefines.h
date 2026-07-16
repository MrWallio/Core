#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

#ifndef INDEX_NONE
#define INDEX_NONE -1
#endif

#define UE_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define PURE_VIRTUAL(func, ...) { __VA_ARGS__ }

#define UE_DEPRECATED(Version, Message) [[deprecated(Message " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.")]]

#define UE_NONCOPYABLE(TypeName) \
	TypeName(TypeName&&) = delete; \
	TypeName(const TypeName&) = delete; \
	TypeName& operator=(const TypeName&) = delete; \
	TypeName& operator=(TypeName&&) = delete;

#define UE_NODISCARD [[nodiscard]]

#define UE_NORETURN [[noreturn]]

#define PREPROCESSOR_JOIN_INNER(x, y) x##y
#define PREPROCESSOR_JOIN(x, y) PREPROCESSOR_JOIN_INNER(x, y)

#define ANONYMOUS_VARIABLE(Name) PREPROCESSOR_JOIN(Name, __LINE__)
