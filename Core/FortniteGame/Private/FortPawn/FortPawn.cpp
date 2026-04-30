#include "pch.h"
#include "FortniteGame/Public/FortPawn/FortPawn.h"

AFortWeapon* AFortPawn::EquipWeaponDefinition(const UFortWeaponItemDefinition* WeaponData, const FGuid& ItemEntryGuid)
{
	if (!this) {
		Log("EquipWeaponDefinition: This is null!");
		return nullptr;
	}

	if (!WeaponData) {
		Log("EquipWeaponDefinition: WeaponData is null!");
		return nullptr;
	}

	static UFunction* Func = nullptr;

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

	ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

void AFortPawn::OnRep_CurrentWeapon(AFortWeapon* OldWeapon)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentWeapon");

	if (!Func) {
		Log("OnRep_CurrentWeapon: Failed to find function!");
		return;
	}

	struct FortPawn_OnRep_CurrentWeapon
	{
	public:
		AFortWeapon* OldWeapon;
	};

	FortPawn_OnRep_CurrentWeapon Parms{};

	Parms.OldWeapon = OldWeapon;

	ProcessEvent(Func, &Parms);
}