#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

struct FCrc
{
	static constexpr std::array<uint32, 256> CRCTablesSB8_0 = []
	{
		std::array<uint32, 256> Table{};
		for (uint32 i = 0; i < 256; ++i)
		{
			uint32 Crc = i;
			for (int32 k = 0; k < 8; ++k)
			{
				Crc = (Crc & 1) ? (0xEDB88320u ^ (Crc >> 1)) : (Crc >> 1);
			}
			Table[i] = Crc;
		}
		return Table;
	}();

	static constexpr std::array<uint32, 256> CRCTable_DEPRECATED = []
	{
		std::array<uint32, 256> Table{};
		for (uint32 i = 0; i < 256; ++i)
		{
			uint32 Crc = i << 24;
			for (int32 k = 0; k < 8; ++k)
			{
				Crc = (Crc & 0x80000000u) ? ((Crc << 1) ^ 0x04C11DB7u) : (Crc << 1);
			}
			Table[i] = Crc;
		}
		return Table;
	}();

	static uint32 MemCrc32(const void* InData, int32 Length, uint32 CRC = 0)
	{
		CRC = ~CRC;
		const uint8* Data = (const uint8*)InData;
		for (int32 i = 0; i < Length; ++i)
		{
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Data[i]) & 0xFF];
		}
		return ~CRC;
	}

	static uint32 MemCrc_DEPRECATED(const void* InData, int32 Length, uint32 CRC = 0)
	{
		CRC = ~CRC;
		const uint8* Data = (const uint8*)InData;
		for (int32 i = 0; i < Length; ++i)
		{
			CRC = (CRC << 8) ^ CRCTable_DEPRECATED[(CRC >> 24) ^ Data[i]];
		}
		return ~CRC;
	}

	static uint32 StrCrc32(const wchar_t* Data, uint32 CRC = 0)
	{
		CRC = ~CRC;
		while (uint32 Ch = (uint32)(uint16)*Data++)
		{
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF]; Ch >>= 8;
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF]; Ch >>= 8;
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF]; Ch >>= 8;
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF];
		}
		return ~CRC;
	}

	static uint32 StrCrc32(const char* Data, uint32 CRC = 0)
	{
		CRC = ~CRC;
		while (uint32 Ch = (uint32)(uint8)*Data++)
		{
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF]; Ch >>= 8;
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF]; Ch >>= 8;
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF]; Ch >>= 8;
			CRC = (CRC >> 8) ^ CRCTablesSB8_0[(CRC ^ Ch) & 0xFF];
		}
		return ~CRC;
	}

	static uint32 Strihash_DEPRECATED(const wchar_t* Data)
	{
		uint32 Hash = 0;
		while (*Data)
		{
			wchar_t Ch = (wchar_t)::towupper(*Data++);
			uint16 B = (uint16)Ch;
			Hash = ((Hash >> 8) & 0x00FFFFFF) ^ CRCTable_DEPRECATED[(Hash ^ B) & 0x000000FF];
			B = (uint16)((uint16)Ch >> 8);
			Hash = ((Hash >> 8) & 0x00FFFFFF) ^ CRCTable_DEPRECATED[(Hash ^ B) & 0x000000FF];
		}
		return Hash;
	}

	static uint32 Strihash_DEPRECATED(const char* Data)
	{
		uint32 Hash = 0;
		while (*Data)
		{
			uint8 B = (uint8)::toupper((uint8)*Data++);
			Hash = ((Hash >> 8) & 0x00FFFFFF) ^ CRCTable_DEPRECATED[(Hash ^ B) & 0x000000FF];
		}
		return Hash;
	}
};
