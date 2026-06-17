#include "pch.h"
#include "FortniteGame/Public/FortPawn/FortPawn.h"

#include "FortniteGame/Public/FortSet/FortHealthSet.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"

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

void AFortPawn::SetHealth(float NewHealthVal)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetHealth");

	struct FortPawn_SetHealth
	{
	public:
		float NewHealthVal;
	};

	FortPawn_SetHealth Parms{};

	Parms.NewHealthVal = NewHealthVal;

	ProcessEvent(Func, &Parms);
}

void AFortPawn::SetMaxHealth(float NewHealthVal)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetMaxHealth");

	struct FortPawn_SetMaxHealth
	{
	public:
		float NewHealthVal;
	};

	FortPawn_SetMaxHealth Parms{};

	Parms.NewHealthVal = NewHealthVal;
	
	ProcessEvent(Func, &Parms);
}

void AFortPawn::SetShield(float NewShieldValue)
{
	if (Version::Fortnite_Version > 1.91) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("SetShield");

		struct FortPawn_SetShield
		{
		public:
			float NewShieldValue;
		};

		FortPawn_SetShield Parms{};

		Parms.NewShieldValue = NewShieldValue;

		ProcessEvent(Func, &Parms);
	}
	else {
		if (HealthSet) {
			HealthSet->CurrentShield.BaseValue = NewShieldValue;
			HealthSet->CurrentShield.CurrentValue = NewShieldValue;
			HealthSet->OnRep_CurrentShield();
			ForceNetUpdate();
		}
		else {
			Log("SetShield: HealthSet is null!");
		}
	}
}

void AFortPawn::SetMaxShield(float NewValue)
{
	if (Version::Fortnite_Version > 1.91) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("SetMaxShield");

		struct FortPawn_SetMaxShield
		{
		public:
			float NewValue;
		};

		FortPawn_SetMaxShield Parms{};

		Parms.NewValue = NewValue;

		ProcessEvent(Func, &Parms);
	}
	else {
		if (HealthSet) {
			HealthSet->Shield.BaseValue = NewValue;
			HealthSet->Shield.CurrentValue = NewValue;
			HealthSet->Shield.Maximum = NewValue;
			HealthSet->OnRep_Shield();
			ForceNetUpdate();
		}
		else {
			Log("SetMaxShield: HealthSet is null!");
		}
	}
}

float AFortPawn::GetMaxHealth() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetMaxHealth");

	struct FortPawn_GetMaxHealth
	{
	public:
		float ReturnValue;
	};

	FortPawn_GetMaxHealth Parms{};

	const_cast<AFortPawn*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

float AFortPawn::GetMaxShield() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetMaxShield");

	struct FortPawn_GetMaxShield
	{
	public:
		float ReturnValue;
	};

	FortPawn_GetMaxShield Parms{};

	const_cast<AFortPawn*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

AFortWeapon* AFortPawn::FindWeapon(UFortItemDefinition* ItemDef)
{
	if (!ItemDef) {
		Log("FindWeapon: ItemDef is null!");
		return nullptr;
	}

	if (!CurrentWeaponList.IsValid()) {
		Log("FindWeapon: CurrentWeaponList is not valid!");
		return nullptr;
	}

	for (AFortWeapon* Weapon : CurrentWeaponList) {
		if (Weapon && Weapon->WeaponData && Weapon->WeaponData == ItemDef) {
			return Weapon;
		}
	}

	return nullptr;
}

AFortWeapon* AFortPawn::FindWeapon(FGuid Guid)
{
	if (!Guid.IsValid()) {
		Log("FindWeapon: Guid is not valid!");
		return nullptr;
	}

	if (!CurrentWeaponList.IsValid()) {
		Log("FindWeapon: CurrentWeaponList is not valid!");
		return nullptr;
	}

	for (AFortWeapon* Weapon : CurrentWeaponList) {
		if (Weapon && Weapon->ItemEntryGuid == Guid) {
			return Weapon;
		}
	}

	return nullptr;
}