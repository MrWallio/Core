#include "pch.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"

void AFortWeap_BuildingTool::OnRep_DefaultMetadata(UBuildingEditModeMetadata* OldMetadata)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_DefaultMetadata");

	struct FortWeap_BuildingTool_OnRep_DefaultMetadata final
	{
	public:
		UBuildingEditModeMetadata* OldMetadata;
	};

	FortWeap_BuildingTool_OnRep_DefaultMetadata Parms{};

	Parms.OldMetadata = OldMetadata;

	ProcessEvent(Func, &Parms);
}