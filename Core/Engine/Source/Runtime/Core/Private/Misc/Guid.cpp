#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"

FString FGuid::ToString(EGuidFormats Format) const
{
	FString(*ToStringInternal)(const FGuid*, EGuidFormats) = decltype(ToStringInternal)(ImageBase + Finder::FindFGuid_ToString());
	return ToStringInternal(this, Format);
}