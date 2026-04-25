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
		// Call GetMaxStackSize function
		return -1;
	}
}

bool UFortItemDefinition::IsStackable() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"IsStackable"));

	struct FortItemDefinition_IsStackable
	{
	public:
		bool ReturnValue;
	};

	FortItemDefinition_IsStackable Parms{};

	const_cast<UFortItemDefinition*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}