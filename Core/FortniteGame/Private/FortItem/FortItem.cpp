#include "pch.h"
#include "FortniteGame/Public/FortItem/FortItem.h"

void UFortItem::SetOwningControllerForTemporaryItem(AFortPlayerController* InController)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"SetOwningControllerForTemporaryItem"));

	struct FortItem_SetOwningControllerForTemporaryItem final
	{
	public:
		AFortPlayerController* InController;
	};

	FortItem_SetOwningControllerForTemporaryItem Parms{};

	Parms.InController = InController;

	ProcessEvent(Func, &Parms);
}

FGuid UFortItem::GetItemGuid() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetItemGuid"));

	struct FortItem_GetItemGuid final
	{
	public:
		FGuid ReturnValue;
	};

	FortItem_GetItemGuid Parms{};

	const_cast<UFortItem*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}