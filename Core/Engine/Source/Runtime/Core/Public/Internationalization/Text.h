#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

class ITextData;

class FText
{
public:
	ITextData* TextData;
	void* ReferenceController;
	uint32 Flags;

public:
	FText()
		: TextData(nullptr), ReferenceController(nullptr), Flags(0)
	{
	}

public:
	inline bool HasTextData() const
	{
		return TextData != nullptr;
	}
};

static_assert(sizeof(FText) == 0x18, "FText layout broke: UE 4.15-5.x expects TSharedRef<ITextData> + Flags = 0x18 on x64");
