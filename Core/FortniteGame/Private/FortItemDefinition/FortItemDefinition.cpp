#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

class UFortItem* UFortItemDefinition::CreateTemporaryItemInstanceBP(int32 Count, int32 Level) const
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"CreateTemporaryItemInstanceBP"));

	struct FortItemDefinition_CreateTemporaryItemInstanceBP
	{
	public:
		int32 Count;
		int32 Level;
		UFortItem* ReturnValue;
	};

	FortItemDefinition_CreateTemporaryItemInstanceBP Parms{};

	Parms.Count = Count;
	Parms.Level = Level;

	const_cast<UFortItemDefinition*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UFortItemDefinition::GetMaxStackSize() const
{
	static UProperty* Prop = StaticClass()->FindPropertyByName("MaxStackSize");
	if (Prop)
	{
		int32 Size = Prop->ElementSize;
		int32 Offset = Prop->Offset_Internal;
		if (Size == 4) {
			return GetFromOffset<int32>(this, Offset);
		}

		return (int32)MaxStackSize.Evaluate();
	}
	else
	{
		static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetMaxStackSize"));
		if (Function) {
			static uintptr_t VTableIdx = GetVTableIndex(Function);

			int32 (*&GetMaxStackSizeInternal)(const UFortItemDefinition*) = decltype(GetMaxStackSizeInternal)(VTable[VTableIdx]);
			return GetMaxStackSizeInternal(this);
		}

		return -1;
	}
}

bool UFortItemDefinition::IsStackable() const
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"IsStackable"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		bool (*&IsStackableInternal)(const UFortItemDefinition*) = decltype(IsStackableInternal)(VTable[VTableIdx]);
		return IsStackableInternal(this);
	}

	return false;
}