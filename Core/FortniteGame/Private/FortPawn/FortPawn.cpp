#include "pch.h"
#include "FortniteGame/Public/FortPawn/FortPawn.h"

AFortWeapon* AFortPawn::EquipWeaponDefinition(const UFortWeaponItemDefinition* WeaponData, const FGuid& ItemEntryGuid)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"EquipWeaponDefinition"));

	struct FortPawn_EquipWeaponDefinition
	{
	public:
		const UFortWeaponItemDefinition* WeaponData;
		FGuid ItemEntryGuid;
		AFortWeapon* ReturnValue;
	};

	FortPawn_EquipWeaponDefinition Parms{};

	Parms.WeaponData = WeaponData;
	Parms.ItemEntryGuid = std::move(ItemEntryGuid);

	UObject::ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}