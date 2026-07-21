#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"

FString FGuid::ToString(EGuidFormats Format) const
{
	switch (Format)
	{
	case EGuidFormats::DigitsWithHyphens:
		return FString::Printf(L"%08X-%04X-%04X-%04X-%04X%08X", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	case EGuidFormats::DigitsWithHyphensInBraces:
		return FString::Printf(L"{%08X-%04X-%04X-%04X-%04X%08X}", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	case EGuidFormats::DigitsWithHyphensInParentheses:
		return FString::Printf(L"(%08X-%04X-%04X-%04X-%04X%08X)", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	case EGuidFormats::HexValuesInBraces:
		return FString::Printf(L"{0x%08X,0x%04X,0x%04X,{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}}",
			A, B >> 16, B & 0xFFFF,
			C >> 24, (C >> 16) & 0xFF, (C >> 8) & 0xFF, C & 0xFF,
			D >> 24, (D >> 16) & 0xFF, (D >> 8) & 0xFF, D & 0xFF);

	case EGuidFormats::UniqueObjectGuid:
		return FString::Printf(L"%08X-%08X-%08X-%08X", A, B, C, D);

	case EGuidFormats::Digits:
	default:
		return FString::Printf(L"%08X%08X%08X%08X", A, B, C, D);
	}
}