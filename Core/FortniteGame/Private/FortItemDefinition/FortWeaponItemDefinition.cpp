#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

FFortBaseWeaponStats* UFortWeaponItemDefinition::GetWeaponStats() const {
	FFortBaseWeaponStats WeaponStats;
	if (UFortKismetLibrary::GetWeaponStatsRow(WeaponStatHandle, &WeaponStats)) {
		return &WeaponStats;
	}

	Log("UFortWeaponItemDefinition::GetWeaponStats: GetWeaponStatsRow Failed!");
	return nullptr;
}

FFortRangedWeaponStats* UFortWeaponItemDefinition::GetRangedWeaponStats() const {
	if (!IsA(UFortWeaponRangedItemDefinition::StaticClass()))
		return nullptr;

	return (FFortRangedWeaponStats*)GetWeaponStats();
}

int32 UFortWeaponItemDefinition::GetClipSize(int32 Level) const {
	FFortBaseWeaponStats* WeaponStats = GetWeaponStats();
	if (!WeaponStats)
		return 1;

	return WeaponStats->GetClipSize(Level);
}

float UFortWeaponItemDefinition::GetDurability(int32 Level) const {
	if (Finder::FindUFortWeaponItemDefinition_GetMaxDurabilityVFT()) {
		float (*&GetMaxDurabilityInternal)(const UFortWeaponItemDefinition* This, int32 Level) = decltype(GetMaxDurabilityInternal)(VTable[Finder::FindUFortWeaponItemDefinition_GetMaxDurabilityVFT()]);
		return GetMaxDurabilityInternal(this, Level);
	}
	else {
		FFortBaseWeaponStats* WeaponStats = GetWeaponStats();
		if (!WeaponStats)
			return 1;

		return WeaponStats->GetDurability(Level, Rarity);
	}
}

TSubclassOf<AFortWeapon> UFortWeaponItemDefinition::GetWeaponActorClass() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetWeaponActorClass");

	struct FortWeaponItemDefinition_GetWeaponActorClass
	{
	public:
		TSubclassOf<AFortWeapon> ReturnValue;
	};

	FortWeaponItemDefinition_GetWeaponActorClass Parms{};

	const_cast<UFortWeaponItemDefinition*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}