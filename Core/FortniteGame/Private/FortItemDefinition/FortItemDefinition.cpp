#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"

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

int32 UFortItemDefinition::GetClipSize() {
	auto RangedDef = Cast<UFortWeaponRangedItemDefinition>();
	if (!RangedDef) {
		return 1;
	}

	auto DataTable = RangedDef->WeaponStatHandle.DataTable;
	auto RowName = RangedDef->WeaponStatHandle.RowName;

	if (!DataTable || !RowName.IsNone()) {
		return 1;
	}

	auto& RowMap = DataTable->RowMap;

	for (int i = 0; i < RowMap.Num(); i++)
	{
		auto& Pair = RowMap[i];

		FName CurrentRowName = Pair.Key();
		FFortRangedWeaponStats* PackageData = (FFortRangedWeaponStats*)Pair.Value();

		if (CurrentRowName == RowName && PackageData)
		{
			return PackageData->ClipSize;
		}
	}

	Log("GetClipSize: Row not found in DataTable or PackageData is null.");
	return 1;
}

EFortQuickBars UFortItemDefinition::GetQuickBarForItem() {
	if (IsA(UFortEditToolItemDefinition::StaticClass())
		|| IsA(UFortBuildingItemDefinition::StaticClass())
		|| IsA(UFortAmmoItemDefinition::StaticClass())
		|| IsA(UFortResourceItemDefinition::StaticClass())
		|| IsA(UFortTrapItemDefinition::StaticClass()))
		return EFortQuickBars::Secondary;

	return EFortQuickBars::Primary;
}