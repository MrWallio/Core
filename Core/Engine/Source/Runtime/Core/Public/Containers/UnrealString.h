#pragma once
#include "pch.h"

#include "Array.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Crc.h"

namespace ESearchCase
{
	enum Type
	{
		CaseSensitive,
		IgnoreCase,
	};
}

namespace ESearchDir
{
	enum Type
	{
		FromStart,
		FromEnd,
	};
}

class FString
{
private:
	typedef TArray<TCHAR> DataType;
	DataType Data;

public:
	using ElementType = TCHAR;

	FORCEINLINE FString() : Data()
	{
	}

	FORCEINLINE FString(const FString& Other)
		: Data(Other.Data)
	{
	}

	FORCEINLINE FString& operator=(const FString& Other)
	{
		Data = Other.Data;
		return *this;
	}

	FORCEINLINE TCHAR& operator[](int32 Index)
	{
		return Data.GetData()[Index];
	}

	FORCEINLINE const TCHAR& operator[](int32 Index) const
	{
		return Data.GetData()[Index];
	}
public:
	FORCEINLINE bool IsEmpty() const
	{
		return Data.Num() <= 1;
	}

	FORCEINLINE int32 Len() const
	{
		return Data.Num() ? Data.Num() - 1 : 0;
	}

	FORCEINLINE const TCHAR* operator*() const
	{
		return Data.Num() ? Data.GetData() : TEXT("");
	}

	FORCEINLINE DataType& GetCharArray()
	{
		return Data;
	}

	FORCEINLINE const DataType& GetCharArray() const
	{
		return Data;
	}

	int32 Num() const
	{
		return Data.Num();
	}

	FORCEINLINE void Empty(int32 Slack = 0)
	{
		Data.Empty(Slack);
	}

	FORCEINLINE void Reset()
	{
		Data.Reset();
	}

	FString& Append(const TCHAR* Text, int32 Count)
	{
		FString& (*AppendInternal)(FString&, const TCHAR*, int32) = decltype(AppendInternal)(ImageBase + Finder::FindFString_Append());
		return AppendInternal(*this, Text, Count);
	}

	void AppendInt(int32 InNum) {
		void (*AppendIntInternal)(FString&, int32) = decltype(AppendIntInternal)(ImageBase + Finder::FindFString_AppendInt());
		AppendIntInternal(*this, InNum);
	}

	bool ToBool() const {
		bool (*ToBoolInternal)(const FString&) = decltype(ToBoolInternal)(ImageBase + Finder::FindFString_ToBool());
		return ToBoolInternal(*this);
	}

public:
	// Custom
	FString(const wchar_t* Str)
	{
		if (!Str)
			return;

		int32_t Length = (int32_t)wcslen(Str);
		Data.Reserve(Length + 1);

		for (int32_t i = 0; i < Length; ++i)
			Data.Add(Str[i]);

		Data.Add(L'\0');
	}

	FString(int32 InCount, const TCHAR* InSrc)
	{
		if (!InSrc || InCount <= 0)
			return;

		Data.Reserve(InCount + 1);

		for (int32 i = 0; i < InCount; ++i)
			Data.Add(InSrc[i]);

		Data.Add(L'\0');
	}

	FString(const char* Str) {
		if (!Str)
			return;

		int32_t Length = (int32_t)strlen(Str);
		Data.Reserve(Length + 1);

		for (int32_t i = 0; i < Length; ++i)
			Data.Add((wchar_t)Str[i]);

		Data.Add(L'\0');
	}

	FString(const std::string& Str) {
		if (Str.empty())
			return;

		std::wstring WStr(Str.begin(), Str.end());

		int32_t Length = (int32_t)WStr.size();
		Data.Reserve(Length + 1);

		for (int32_t i = 0; i < Length; ++i)
			Data.Add(WStr[i]);

		Data.Add(L'\0');
	}

	std::string ToString() const
	{
		const TCHAR* DataPtr = Data.GetData();

		if (!DataPtr || Data.ArrayNum == 0)
		{
			return std::string();
		}

		size_t Length = std::wcslen(DataPtr);

		if (Length == 0)
		{
			return std::string();
		}

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, DataPtr, (int)Length, NULL, 0, NULL, NULL);
		std::string Result(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, DataPtr, (int)Length, &Result[0], size_needed, NULL, NULL);

		return Result;
	}

	operator std::string() const
	{
		return ToString();
	}

	bool IsValid() const
	{
		return Data.IsValid();
	}

public:
	bool Equals(const FString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive) const
	{
		if (SearchCase == ESearchCase::CaseSensitive)
		{
			return std::wcscmp(**this, *Other) == 0;
		}

		return _wcsicmp(**this, *Other) == 0;
	}

	bool operator==(const FString& Other) const
	{
		return _wcsicmp(**this, *Other) == 0;
	}

	bool operator!=(const FString& Other) const
	{
		return !(*this == Other);
	}

	bool operator<(const FString& Other) const
	{
		return _wcsicmp(**this, *Other) < 0;
	}

	int32 Compare(const FString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive) const
	{
		if (SearchCase == ESearchCase::CaseSensitive)
		{
			return (int32)std::wcscmp(**this, *Other);
		}

		return (int32)_wcsicmp(**this, *Other);
	}

	int32 Find(const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase, ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition = -1) const
	{
		const int32 SubLen = SubStr.Len();
		const int32 ThisLen = Len();

		if (SubLen == 0 || ThisLen < SubLen)
		{
			return -1;
		}

		const TCHAR* Buffer = **this;
		const TCHAR* SubBuffer = *SubStr;

		if (SearchDir == ESearchDir::FromStart)
		{
			int32 Start = StartPosition < 0 ? 0 : StartPosition;
			if (Start > ThisLen - SubLen)
			{
				return -1;
			}

			for (int32 i = Start; i <= ThisLen - SubLen; ++i)
			{
				const bool bMatches = SearchCase == ESearchCase::CaseSensitive
					? std::wcsncmp(Buffer + i, SubBuffer, SubLen) == 0
					: _wcsnicmp(Buffer + i, SubBuffer, SubLen) == 0;

				if (bMatches)
				{
					return i;
				}
			}
		}
		else
		{
			int32 Start = StartPosition < 0 ? ThisLen - SubLen : StartPosition - SubLen;
			if (Start > ThisLen - SubLen)
			{
				Start = ThisLen - SubLen;
			}

			for (int32 i = Start; i >= 0; --i)
			{
				const bool bMatches = SearchCase == ESearchCase::CaseSensitive
					? std::wcsncmp(Buffer + i, SubBuffer, SubLen) == 0
					: _wcsnicmp(Buffer + i, SubBuffer, SubLen) == 0;

				if (bMatches)
				{
					return i;
				}
			}
		}

		return -1;
	}

	bool Contains(const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase, ESearchDir::Type SearchDir = ESearchDir::FromStart) const
	{
		return Find(SubStr, SearchCase, SearchDir) != -1;
	}

	bool StartsWith(const FString& InPrefix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const
	{
		const int32 PrefixLen = InPrefix.Len();
		if (PrefixLen == 0 || PrefixLen > Len())
		{
			return PrefixLen == 0;
		}

		if (SearchCase == ESearchCase::CaseSensitive)
		{
			return std::wcsncmp(**this, *InPrefix, PrefixLen) == 0;
		}

		return _wcsnicmp(**this, *InPrefix, PrefixLen) == 0;
	}

	bool EndsWith(const FString& InSuffix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const
	{
		const int32 SuffixLen = InSuffix.Len();
		const int32 ThisLen = Len();

		if (SuffixLen == 0 || SuffixLen > ThisLen)
		{
			return SuffixLen == 0;
		}

		if (SearchCase == ESearchCase::CaseSensitive)
		{
			return std::wcsncmp(**this + ThisLen - SuffixLen, *InSuffix, SuffixLen) == 0;
		}

		return _wcsnicmp(**this + ThisLen - SuffixLen, *InSuffix, SuffixLen) == 0;
	}

	FString Mid(int32 Start, int32 Count) const
	{
		const int32 ThisLen = Len();

		if (Start < 0)
		{
			Count += Start;
			Start = 0;
		}
		if (Start >= ThisLen || Count <= 0)
		{
			return FString();
		}
		if (Start + Count > ThisLen)
		{
			Count = ThisLen - Start;
		}

		return FString(Count, **this + Start);
	}

	FString Mid(int32 Start) const
	{
		return Mid(Start, Len() - Start);
	}

	FString Left(int32 Count) const
	{
		return Mid(0, Count);
	}

	FString LeftChop(int32 Count) const
	{
		return Mid(0, Len() - Count);
	}

	FString Right(int32 Count) const
	{
		return Mid(Len() - Count, Count);
	}

	FString RightChop(int32 Count) const
	{
		return Mid(Count, Len() - Count);
	}

	FString ToUpper() const
	{
		const int32 ThisLen = Len();
		std::wstring Result(**this, ThisLen);

		for (int32 i = 0; i < ThisLen; ++i)
		{
			Result[i] = (wchar_t)::towupper(Result[i]);
		}

		return FString(Result.c_str());
	}

	FString ToLower() const
	{
		const int32 ThisLen = Len();
		std::wstring Result(**this, ThisLen);

		for (int32 i = 0; i < ThisLen; ++i)
		{
			Result[i] = (wchar_t)::towlower(Result[i]);
		}

		return FString(Result.c_str());
	}

	FString Replace(const FString& From, const FString& To, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const
	{
		const int32 FromLen = From.Len();
		if (FromLen == 0)
		{
			return *this;
		}

		std::wstring Result;
		int32 SearchStart = 0;

		while (true)
		{
			const int32 FoundIndex = Find(From, SearchCase, ESearchDir::FromStart, SearchStart);
			if (FoundIndex == -1)
			{
				Result.append(**this + SearchStart, (size_t)(Len() - SearchStart));
				break;
			}

			Result.append(**this + SearchStart, (size_t)(FoundIndex - SearchStart));
			Result.append(*To, (size_t)To.Len());
			SearchStart = FoundIndex + FromLen;
		}

		return FString(Result.c_str());
	}

	void ReplaceInline(const FString& From, const FString& To, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase)
	{
		*this = Replace(From, To, SearchCase);
	}

	void ToUpperInline()
	{
		TCHAR* Buffer = Data.GetData();
		const int32 ThisLen = Len();

		for (int32 i = 0; i < ThisLen; ++i)
		{
			Buffer[i] = (TCHAR)::towupper(Buffer[i]);
		}
	}

	void ToLowerInline()
	{
		TCHAR* Buffer = Data.GetData();
		const int32 ThisLen = Len();

		for (int32 i = 0; i < ThisLen; ++i)
		{
			Buffer[i] = (TCHAR)::towlower(Buffer[i]);
		}
	}

	FString TrimStart() const
	{
		const int32 ThisLen = Len();
		const TCHAR* Buffer = **this;

		int32 Start = 0;
		while (Start < ThisLen && ::iswspace(Buffer[Start]))
		{
			++Start;
		}

		return Mid(Start, ThisLen - Start);
	}

	FString TrimEnd() const
	{
		const int32 ThisLen = Len();
		const TCHAR* Buffer = **this;

		int32 End = ThisLen;
		while (End > 0 && ::iswspace(Buffer[End - 1]))
		{
			--End;
		}

		return Mid(0, End);
	}

	FString TrimStartAndEnd() const
	{
		const int32 ThisLen = Len();
		const TCHAR* Buffer = **this;

		int32 Start = 0;
		while (Start < ThisLen && ::iswspace(Buffer[Start]))
		{
			++Start;
		}

		int32 End = ThisLen;
		while (End > Start && ::iswspace(Buffer[End - 1]))
		{
			--End;
		}

		return Mid(Start, End - Start);
	}

	void TrimStartAndEndInline()
	{
		*this = TrimStartAndEnd();
	}

	bool FindChar(TCHAR InChar, int32& OutIndex) const
	{
		const TCHAR* Buffer = **this;
		const int32 ThisLen = Len();

		for (int32 i = 0; i < ThisLen; ++i)
		{
			if (Buffer[i] == InChar)
			{
				OutIndex = i;
				return true;
			}
		}

		OutIndex = -1;
		return false;
	}

	bool FindLastChar(TCHAR InChar, int32& OutIndex) const
	{
		const TCHAR* Buffer = **this;

		for (int32 i = Len() - 1; i >= 0; --i)
		{
			if (Buffer[i] == InChar)
			{
				OutIndex = i;
				return true;
			}
		}

		OutIndex = -1;
		return false;
	}

	void InsertAt(int32 Index, TCHAR Character)
	{
		if (Index < 0 || Index > Len())
			return;

		if (Data.Num() == 0)
		{
			AppendChar(Character);
			return;
		}

		Data.Insert(Character, Index);
	}

	void InsertAt(int32 Index, const FString& Characters)
	{
		if (Index < 0 || Index > Len())
			return;

		const TCHAR* Buffer = *Characters;
		for (int32 i = 0; i < Characters.Len(); ++i)
		{
			InsertAt(Index + i, Buffer[i]);
		}
	}

	void RemoveAt(int32 Index, int32 Count = 1)
	{
		if (Index < 0 || Count <= 0 || Index >= Len())
			return;

		if (Index + Count > Len())
		{
			Count = Len() - Index;
		}

		Data.RemoveAt(Index, Count, true);
	}

	bool Split(const FString& InS, FString* LeftS, FString* RightS, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase, ESearchDir::Type SearchDir = ESearchDir::FromStart) const
	{
		const int32 SplitIndex = Find(InS, SearchCase, SearchDir);
		if (SplitIndex == -1)
		{
			return false;
		}

		if (LeftS)
		{
			*LeftS = Left(SplitIndex);
		}
		if (RightS)
		{
			*RightS = Mid(SplitIndex + InS.Len());
		}

		return true;
	}

	bool IsNumeric() const
	{
		const TCHAR* Str = **this;

		if (*Str == L'-' || *Str == L'+')
		{
			Str++;
		}

		bool bHasDot = false;
		while (*Str != L'\0')
		{
			if (*Str == L'.')
			{
				if (bHasDot)
				{
					return false;
				}
				bHasDot = true;
			}
			else if (!::iswdigit(*Str))
			{
				return false;
			}

			++Str;
		}

		return true;
	}

	static FString FromInt(int32 Num)
	{
		return FString(std::to_wstring(Num).c_str());
	}

	static FString SanitizeFloat(double InFloat)
	{
		wchar_t Buffer[64];
		swprintf_s(Buffer, 64, L"%f", InFloat);

		FString Result(Buffer);

		// Trim trailing zeros but keep at least one decimal digit, like UE.
		int32 DotIndex = -1;
		if (Result.FindChar(L'.', DotIndex))
		{
			int32 End = Result.Len();
			while (End > DotIndex + 2 && Result[End - 1] == L'0')
			{
				--End;
			}
			Result = Result.Left(End);
		}

		return Result;
	}

	static FString Printf(const TCHAR* Fmt, ...)
	{
		wchar_t Buffer[4096];

		va_list Args;
		va_start(Args, Fmt);
		_vsnwprintf_s(Buffer, 4096, _TRUNCATE, Fmt, Args);
		va_end(Args);

		return FString(Buffer);
	}

	int32 ParseIntoArray(TArray<FString>& OutArray, const TCHAR* pchDelim, bool InCullEmpty = true) const
	{
		OutArray.Empty();

		const FString Delim(pchDelim);
		const int32 DelimLen = Delim.Len();

		if (DelimLen == 0)
		{
			if (!InCullEmpty || Len() > 0)
			{
				OutArray.Add(*this);
			}
			return OutArray.Num();
		}

		int32 Start = 0;
		while (true)
		{
			const int32 FoundIndex = Find(Delim, ESearchCase::CaseSensitive, ESearchDir::FromStart, Start);
			if (FoundIndex == -1)
			{
				FString Piece = Mid(Start, Len() - Start);
				if (!InCullEmpty || Piece.Len() > 0)
				{
					OutArray.Add(Piece);
				}
				break;
			}

			FString Piece = Mid(Start, FoundIndex - Start);
			if (!InCullEmpty || Piece.Len() > 0)
			{
				OutArray.Add(Piece);
			}

			Start = FoundIndex + DelimLen;
		}

		return OutArray.Num();
	}

	void AppendChar(TCHAR InChar)
	{
		if (Data.Num() == 0)
		{
			Data.Add(InChar);
			Data.Add(L'\0');
		}
		else
		{
			Data.GetData()[Data.Num() - 1] = InChar;
			Data.Add(L'\0');
		}
	}

	FString& Append(const FString& Other)
	{
		const int32 OtherLen = Other.Len();
		const TCHAR* OtherBuffer = *Other;

		for (int32 i = 0; i < OtherLen; ++i)
		{
			AppendChar(OtherBuffer[i]);
		}

		return *this;
	}

	FString& operator+=(const FString& Other)
	{
		return Append(Other);
	}

	FString& operator+=(const TCHAR* Str)
	{
		return Append(FString(Str));
	}

	FString operator+(const FString& Other) const
	{
		FString Result(*this);
		Result.Append(Other);
		return Result;
	}

	FString operator+(const TCHAR* Str) const
	{
		FString Result(*this);
		Result.Append(FString(Str));
		return Result;
	}

	friend FString operator+(const TCHAR* Lhs, const FString& Rhs)
	{
		FString Result(Lhs);
		Result.Append(Rhs);
		return Result;
	}
};

FORCEINLINE uint32 GetTypeHash(const FString& S)
{
	return FCrc::Strihash_DEPRECATED(*S);
}

static_assert(sizeof(FString) == 0x10, "FString layout broke: UE 4.0-5.x expects a single TArray<TCHAR> = 0x10 on x64");
