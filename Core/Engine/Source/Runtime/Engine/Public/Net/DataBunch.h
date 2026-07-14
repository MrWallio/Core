#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/CoreNet.h"

class UChannel;
class UNetConnection;

class alignas(16) FOutBunch : public FNetBitWriter
{
private:
	uint8 Padding[0x300];

public:
	FOutBunch(class UChannel* InChannel, bool bClose);

public:
	bool GetbDormant() {
		if (Version::Engine_Version == 4.16) {
			return *((bool*)(uintptr_t)this + 0xE6);
		}

		return 0;
	}

	bool GetbReliable() {
		if (Version::Engine_Version == 4.16) {
			return *((bool*)(uintptr_t)this + 0xE8);
		}

		return 0;
	}

	void SetbDormant(bool Value) {
		if (Version::Engine_Version == 4.16) {
			*((bool*)(uintptr_t)this + 0xE6) = Value;
		}
	}

	void SetbReliable(bool Value) {
		if (Version::Engine_Version == 4.16) {
			*((bool*)(uintptr_t)this + 0xE8) = Value;
		}
	}
};
