#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"

bool UFortWorldItemDefinition::ServerExecute(UFortItem* Item, AFortPlayerController* Instigator) {
	bool (*&ServerExecuteInternal)(UFortWorldItemDefinition * This, UFortItem * Item, AFortPlayerController * Instigator) = decltype(ServerExecuteInternal)(VTable[Finder::FindUFortWorldItemDefinition_ServerExecuteVFT()]);
	return ServerExecuteInternal(this, Item, Instigator);
}

UFortWorldItemDefinition* UFortWorldItemDefinition::GetAmmoWorldItemDefinition_BP() const
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetAmmoWorldItemDefinition_BP");

	struct FortWorldItemDefinition_GetAmmoWorldItemDefinition_BP final
	{
	public:
		UFortWorldItemDefinition* ReturnValue;
	};

	FortWorldItemDefinition_GetAmmoWorldItemDefinition_BP Parms{};

	const_cast<UFortWorldItemDefinition*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}