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
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetItemGuid"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		FGuid& (*&GetItemGuidInternal)(const UFortItem*) = decltype(GetItemGuidInternal)(VTable[VTableIdx]);
		return GetItemGuidInternal(this);
	}

	return FGuid();
}

AFortPlayerController* UFortItem::GetOwningController() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetOwningController");

	struct FortItem_GetOwningController
	{
	public:
		AFortPlayerController* ReturnValue;
	};

	FortItem_GetOwningController Parms{};

	const_cast<UFortItem*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}