#pragma once
#include "pch.h"

#include <string>

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

class FTCHARToUTF8
{
private:
	std::string Converted;

public:
	explicit FTCHARToUTF8(const TCHAR* Str)
	{
		if (!Str || !*Str)
			return;

		const int32 SourceLen = (int32)wcslen(Str);
		const int32 NeededLen = WideCharToMultiByte(CP_UTF8, 0, Str, SourceLen, nullptr, 0, nullptr, nullptr);

		Converted.resize((size_t)NeededLen);
		WideCharToMultiByte(CP_UTF8, 0, Str, SourceLen, Converted.data(), NeededLen, nullptr, nullptr);
	}

	inline const char* Get() const { return Converted.c_str(); }
	inline int32 Length() const { return (int32)Converted.size(); }
};

class FUTF8ToTCHAR
{
private:
	std::wstring Converted;

public:
	explicit FUTF8ToTCHAR(const char* Str)
	{
		if (!Str || !*Str)
			return;

		const int32 SourceLen = (int32)strlen(Str);
		const int32 NeededLen = MultiByteToWideChar(CP_UTF8, 0, Str, SourceLen, nullptr, 0);

		Converted.resize((size_t)NeededLen);
		MultiByteToWideChar(CP_UTF8, 0, Str, SourceLen, Converted.data(), NeededLen);
	}

	inline const TCHAR* Get() const { return Converted.c_str(); }
	inline int32 Length() const { return (int32)Converted.size(); }
};

class FTCHARToANSI
{
private:
	std::string Converted;

public:
	explicit FTCHARToANSI(const TCHAR* Str)
	{
		if (!Str || !*Str)
			return;

		const int32 SourceLen = (int32)wcslen(Str);
		const int32 NeededLen = WideCharToMultiByte(CP_ACP, 0, Str, SourceLen, nullptr, 0, nullptr, nullptr);

		Converted.resize((size_t)NeededLen);
		WideCharToMultiByte(CP_ACP, 0, Str, SourceLen, Converted.data(), NeededLen, nullptr, nullptr);
	}

	inline const char* Get() const { return Converted.c_str(); }
	inline int32 Length() const { return (int32)Converted.size(); }
};

class FANSIToTCHAR
{
private:
	std::wstring Converted;

public:
	explicit FANSIToTCHAR(const char* Str)
	{
		if (!Str || !*Str)
			return;

		const int32 SourceLen = (int32)strlen(Str);
		const int32 NeededLen = MultiByteToWideChar(CP_ACP, 0, Str, SourceLen, nullptr, 0);

		Converted.resize((size_t)NeededLen);
		MultiByteToWideChar(CP_ACP, 0, Str, SourceLen, Converted.data(), NeededLen);
	}

	inline const TCHAR* Get() const { return Converted.c_str(); }
	inline int32 Length() const { return (int32)Converted.size(); }
};

#ifndef TCHAR_TO_UTF8
#define TCHAR_TO_UTF8(str) FTCHARToUTF8(str).Get()
#endif

#ifndef UTF8_TO_TCHAR
#define UTF8_TO_TCHAR(str) FUTF8ToTCHAR(str).Get()
#endif

#ifndef TCHAR_TO_ANSI
#define TCHAR_TO_ANSI(str) FTCHARToANSI(str).Get()
#endif

#ifndef ANSI_TO_TCHAR
#define ANSI_TO_TCHAR(str) FANSIToTCHAR(str).Get()
#endif
