#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h"

bool FURL::IsInternal() const
{
	return Protocol == FString(L"unreal");
}

bool FURL::IsLocalInternal() const
{
	return IsInternal() && Host.Len() == 0;
}

const TCHAR* FURL::GetOption(const TCHAR* Match, const TCHAR* Default) const
{
	const int32 Len = (int32)wcslen(Match);
	if (Len > 0)
	{
		for (int32 i = 0; i < Op.Num(); i++)
		{
			const TCHAR* s = *Op[i];
			if (_wcsnicmp(s, Match, Len) == 0)
			{
				if (s[Len - 1] == L'=' || s[Len] == L'=' || s[Len] == L'\0')
					return s + Len;
			}
		}
	}
	return Default;
}

bool FURL::HasOption(const TCHAR* Test) const
{
	return GetOption(Test, nullptr) != nullptr;
}

void FURL::AddOption(const TCHAR* Str)
{
	const TCHAR* Equals = wcschr(Str, L'=');
	const int32 Match = Equals ? (int32)(Equals - Str) : (int32)wcslen(Str);

	int32 i;
	for (i = 0; i < Op.Num(); i++)
	{
		const TCHAR* s = *Op[i];
		if (_wcsnicmp(s, Str, Match) == 0 && (s[Match] == L'=' || s[Match] == L'\0'))
			break;
	}

	if (i == Op.Num())
		Op.Add(FString(Str));
	else
		Op[i] = FString(Str);
}

void FURL::RemoveOption(const TCHAR* Key, const TCHAR* Section, const FString& Filename)
{
	void (*RemoveOptionInternal)(FURL*, const TCHAR*, const TCHAR*, const FString&) = decltype(RemoveOptionInternal)(ImageBase + Finder::FindFURL_RemoveOption());
	RemoveOptionInternal(this, Key, Section, Filename);
}

FString FURL::ToString(bool FullyQualified) const
{
	FString Result;

	if (Protocol != FString(L"unreal") || FullyQualified)
	{
		Result += Protocol;
		Result += L":";
		if (Host.Len() != 0)
			Result += L"//";
	}

	if (Host.Len() != 0 || Port != 7777)
	{
		Result += Host;
		if (Port != 7777)
		{
			Result += L":";
			Result += FString::FromInt(Port);
		}
		Result += L"/";
	}

	if (Map.Len() > 0)
		Result += Map;

	for (int32 i = 0; i < Op.Num(); i++)
	{
		Result += L"?";
		Result += Op[i];
	}

	if (Portal.Len() > 0)
	{
		Result += L"#";
		Result += Portal;
	}

	return Result;
}

FString FURL::GetHostPortString() const
{
	return Host + FString(L":") + FString::FromInt(Port);
}