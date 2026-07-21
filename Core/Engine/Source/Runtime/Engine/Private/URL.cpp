#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h"

bool FURL::IsInternal() const
{
	bool (*IsInternalInternal)(const FURL*) = decltype(IsInternalInternal)(ImageBase + Finder::FindFURL_IsInternal());
	return IsInternalInternal(this);
}

bool FURL::IsLocalInternal() const
{
	bool (*IsLocalInternalInternal)(const FURL*) = decltype(IsLocalInternalInternal)(ImageBase + Finder::FindFURL_IsLocalInternal());
	return IsLocalInternalInternal(this);
}

bool FURL::HasOption(const TCHAR* Test) const
{
	bool (*HasOptionInternal)(const FURL*, const TCHAR*) = decltype(HasOptionInternal)(ImageBase + Finder::FindFURL_HasOption());
	return HasOptionInternal(this, Test);
}

const TCHAR* FURL::GetOption(const TCHAR* Match, const TCHAR* Default) const
{
	const TCHAR* (*GetOptionInternal)(const FURL*, const TCHAR*, const TCHAR*) = decltype(GetOptionInternal)(ImageBase + Finder::FindFURL_GetOption());
	return GetOptionInternal(this, Match, Default);
}

void FURL::AddOption(const TCHAR* Str)
{
	void (*AddOptionInternal)(FURL*, const TCHAR*) = decltype(AddOptionInternal)(ImageBase + Finder::FindFURL_AddOption());
	AddOptionInternal(this, Str);
}

void FURL::RemoveOption(const TCHAR* Key, const TCHAR* Section, const FString& Filename)
{
	void (*RemoveOptionInternal)(FURL*, const TCHAR*, const TCHAR*, const FString&) = decltype(RemoveOptionInternal)(ImageBase + Finder::FindFURL_RemoveOption());
	RemoveOptionInternal(this, Key, Section, Filename);
}

FString FURL::ToString(bool FullyQualified) const
{
	FString& (*ToStringInternal)(const FURL*, FString&, bool) = decltype(ToStringInternal)(ImageBase + Finder::FindFURL_ToString());
	FString Result = FString();
	return ToStringInternal(this, Result, FullyQualified);
}

FString FURL::GetHostPortString() const
{
	FString& (*GetHostPortStringInternal)(const FURL*, FString&) = decltype(GetHostPortStringInternal)(ImageBase + Finder::FindFURL_GetHostPortString());
	FString Result = FString();
	return GetHostPortStringInternal(this, Result);
}