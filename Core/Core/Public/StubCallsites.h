#pragma once
#include "pch.h"

#include <functional>
#include <vector>

namespace StubCallsites {
	inline uintptr_t FromString(const wchar_t* String)
	{
		auto Ref = Memcury::Scanner::FindStringRef(String, false);
		return Ref.IsValid() ? Ref.FindFunctionStart().Get() : 0;
	}

	inline uintptr_t FromReflection(const char* FunctionPath)
	{
		auto Fn = (UFunction*)FUObjectArray::FindObject(FunctionPath);
		if (!Fn || !Fn->Func)
			return 0;

		uintptr_t Func = (uintptr_t)Fn->Func;

		if (Memcury::Scanner(Func).FindFunctionEnd().Get() > Func)
			return Func;

		for (int i = 0; i < 0x40; i++)
		{
			uintptr_t Cursor = Func + i;

			if (*(uint8*)Cursor == 0xE9)
				return Memcury::PE::Address(Cursor).RelativeOffset(1).Get();
		}

		return Func;
	}

	inline uintptr_t FromVTable(UClass* Class, uintptr_t Index)
	{
		if (!Class || !Index)
			return 0;

		return (uintptr_t)Class->GetDefaultObject()->VTable[Index];
	}

	inline uintptr_t FromOffset(uintptr_t Offset)
	{
		return Offset ? Offset + ImageBase : 0;
	}

	inline uintptr_t FromSignature(const char* Signature)
	{
		return Memcury::Scanner::FindPattern(Signature, false).Get();
	}

	inline uintptr_t FromXref(const char* CalleeSignature)
	{
		uintptr_t Callee = Memcury::Scanner::FindPattern(CalleeSignature, false).Get();
		if (!Callee)
			return 0;

		auto Refs = FindBranchRefsToAddress(Callee);
		if (Refs.size() != 1)
			return 0;

		return Memcury::Scanner(Refs[0]).FindFunctionStart().Get();
	}

	inline uintptr_t ResolveStub(uintptr_t Function)
	{
		if (!Function)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();

		for (uintptr_t Cursor = Function; Cursor + 5 <= End; Cursor++)
		{
			if (*(uint8*)Cursor != 0xE8)
				continue;

			uintptr_t Target = Memcury::PE::Address(Cursor).RelativeOffset(1).Get();
			if (IsReturnNullStub(Target))
				return Target;
		}

		return 0;
	}

	inline uintptr_t FindCall(uintptr_t Function, uintptr_t Stub, int Depth = 2)
	{
		if (!Function || !Stub || Depth < 0)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();
		if (End <= Function)
			return 0;

		if (uintptr_t Addr = FindCallRefInRange(Function, End, Stub))
			return Addr;

		if (Depth == 0)
			return 0;

		for (uintptr_t Cursor = Function; Cursor + 5 <= End; Cursor++)
		{
			if (*(uint8*)Cursor != 0xE8)
				continue;

			uintptr_t Callee = Memcury::PE::Address(Cursor).RelativeOffset(1).Get();

			if (uintptr_t Addr = FindCall(Callee, Stub, Depth - 1))
				return Addr;
		}

		return 0;
	}

	inline uintptr_t ResolveEmptyStub(uintptr_t Function)
	{
		if (!Function)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();

		for (uintptr_t Cursor = Function; Cursor + 7 <= End; Cursor++)
		{
			auto* b = reinterpret_cast<const uint8_t*>(Cursor);

			if ((b[0] != 0x48 && b[0] != 0x4C) || b[1] != 0x8D || (b[2] & 0xC7) != 0x05)
				continue;

			uintptr_t Target = Cursor + 7 + *reinterpret_cast<const int32_t*>(Cursor + 3);
			if (IsEmptyStub(Target))
				return Target;
		}

		return 0;
	}

	inline bool IsBoundIntoObject(uintptr_t Addr)
	{
		auto* b = reinterpret_cast<const uint8_t*>(Addr);

		int LeaReg = ((b[2] >> 3) & 7) + (b[0] == 0x4C ? 8 : 0);

		for (uintptr_t Cursor = Addr + 7; Cursor < Addr + 7 + 0x20; Cursor++)
		{
			auto* m = reinterpret_cast<const uint8_t*>(Cursor);

			if ((m[0] != 0x48 && m[0] != 0x4C) || m[1] != 0x89)
				continue;

			int Mod = (m[2] >> 6) & 3;
			int Reg = ((m[2] >> 3) & 7) + (m[0] == 0x4C ? 8 : 0);
			int Rm = m[2] & 7;

			if (Reg != LeaReg)
				continue;

			if ((Mod == 1 || Mod == 2) && Rm != 4)
				return true;
		}

		return false;
	}

	inline uintptr_t FindLea(uintptr_t Function, uintptr_t Stub, int Depth = 2)
	{
		if (!Function || !Stub || Depth < 0)
			return 0;

		uintptr_t End = Memcury::Scanner(Function).FindFunctionEnd().Get();
		if (End <= Function)
			return 0;

		uintptr_t Fallback = 0;

		for (uintptr_t Cursor = Function; Cursor < End; )
		{
			uintptr_t Addr = FindLeaRefInRange(Cursor, End, Stub);
			if (!Addr)
				break;

			if (IsBoundIntoObject(Addr))
				return Addr;

			if (!Fallback)
				Fallback = Addr;

			Cursor = Addr + 1;
		}

		if (Fallback)
			return Fallback;

		if (Depth == 0)
			return 0;

		for (uintptr_t Cursor = Function; Cursor + 5 <= End; Cursor++)
		{
			if (*(uint8*)Cursor != 0xE8)
				continue;

			uintptr_t Callee = Memcury::PE::Address(Cursor).RelativeOffset(1).Get();

			if (uintptr_t Addr = FindLea(Callee, Stub, Depth - 1))
				return Addr;
		}

		return 0;
	}

	using FLocator = std::function<uintptr_t()>;

	inline FLocator ByString(const wchar_t* String) { return [=] { return FromString(String); }; }
	inline FLocator ByReflection(const char* FunctionPath) { return [=] { return FromReflection(FunctionPath); }; }
	inline FLocator ByVTable(UClass* Class, uintptr_t Index) { return [=] { return FromVTable(Class, Index); }; }
	inline FLocator ByOffset(uintptr_t Offset) { return [=] { return FromOffset(Offset); }; }
	inline FLocator BySignature(const char* Signature) { return [=] { return FromSignature(Signature); }; }
	inline FLocator ByXref(const char* CalleeSignature) { return [=] { return FromXref(CalleeSignature); }; }

	struct FSite {
		const char* Name;
		std::vector<FLocator> Locators;
	};

	inline void Patch(const char* Label, uintptr_t Stub, void* Detour, std::initializer_list<FSite> Sites, bool bWarnIfNotFound = true)
	{
		for (const auto& Site : Sites)
		{
			uintptr_t Addr = 0;

			for (const auto& Locator : Site.Locators)
			{
				uintptr_t Function = Locator();
				if (!Function)
					continue;

				Addr = FindCall(Function, Stub);
				if (Addr)
					break;
			}

			if (!Addr) {
				if (bWarnIfNotFound) {
					Log(std::string("Failed to find patch for ") + Label + ": " + Site.Name);
				}
				continue;
			}

			Log(std::string(Label) + " Patch: " + Site.Name + " @ 0x" + std::format("{:X}", Addr - ImageBase));
			PatchCallFar(Addr, Detour);
		}
	}

	inline void PatchBound(const char* Label, uintptr_t Stub, void* Detour, std::initializer_list<FSite> Sites, bool bWarnIfNotFound = true)
	{
		for (const auto& Site : Sites)
		{
			uintptr_t Addr = 0;

			for (const auto& Locator : Site.Locators)
			{
				uintptr_t Function = Locator();
				if (!Function)
					continue;

				Addr = FindLea(Function, Stub);
				if (Addr)
					break;
			}

			if (!Addr) {
				if (bWarnIfNotFound) {
					Log(std::string("Failed to find patch for ") + Label + ": " + Site.Name);
				}
				continue;
			}

			Log(std::string(Label) + " Patch: " + Site.Name + " @ 0x" + std::format("{:X}", Addr - ImageBase));
			PatchLeaFar(Addr, Detour);
		}
	}
}
