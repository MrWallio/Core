#include "pch.h"
#include "../Public/AbilitySystemComponent.h"

#include "../Public/Abilities/GameplayAbility.h"

void UAbilitySystemComponent::ClientActivateAbilityFailed(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey) {
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ClientActivateAbilityFailed"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ClientActivateAbilityFailedInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, int16) = decltype(ClientActivateAbilityFailedInternal)(VTable[VTableIdx]);
		ClientActivateAbilityFailedInternal(this, AbilityToActivate, PredictionKey);
	}
}

FGameplayAbilitySpec* UAbilitySystemComponent::FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle)
{
	FGameplayAbilitySpec* (*FindAbilitySpecFromHandleInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle) = decltype(FindAbilitySpecFromHandleInternal)(ImageBase + Finder::FindUAbilitySystemComponent_FindAbilitySpecFromHandle());
	return FindAbilitySpecFromHandleInternal(this, Handle);
}

void UAbilitySystemComponent::InternalServerTryActivateAbility(FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData) {
	if (Version::Engine_Version == 4.16) {
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
		if (!Spec)
		{
			ClientActivateAbilityFailed(Handle, PredictionKey.Current);
			return;
		}

		ConsumeAllReplicatedData(Handle, PredictionKey);

		const UGameplayAbility* AbilityToActivate = Spec->Ability;
		if (!AbilityToActivate)
		{
			return;
		}

		UGameplayAbility* InstancedAbility = nullptr;
		Spec->InputPressed = true;

		if (InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
		{
			// TryActivateAbility handles notifying the client of success
		}
		else
		{
			Log("InternalServerTryActiveAbility. Rejecting ClientActivation of " + Spec->Ability->GetName().ToString() + ". InternalTryActivateAbility failed.");
			ClientActivateAbilityFailed(Handle, PredictionKey.Current);
			Spec->InputPressed = false;
		}
		ActivatableAbilities.MarkItemDirty(*Spec);
	}
}

void UAbilitySystemComponent::InternalServerTryActivateAbilityHK(UAbilitySystemComponent* This, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData) {
	if (!This) return InternalServerTryActivateAbilityOG(This, Handle, InputPressed, PredictionKey, TriggerEventData);

	return This->InternalServerTryActivateAbility(Handle, InputPressed, PredictionKey, TriggerEventData);
}

void UAbilitySystemComponent::ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey)
{
	// This is not stripped so i can find the offset at some point
}

bool UAbilitySystemComponent::InternalTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey InPredictionKey, UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	bool (*InternalTryActivateAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey, UGameplayAbility**, void*, const FGameplayEventData*) = decltype(InternalTryActivateAbilityInternal)(ImageBase + Finder::FindUAbilitySystemComponent_InternalTryActivateAbility());
	return InternalTryActivateAbilityInternal(this, AbilityToActivate, InPredictionKey, OutInstancedAbility, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::GiveAbility(FGameplayAbilitySpec& AbilitySpec) {
	static void (*GiveAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityInternal)(ImageBase + Finder::FindUAbilitySystemComponent_GiveAbility());
	FGameplayAbilitySpecHandle OutHandle;
	GiveAbilityInternal(this, &OutHandle, AbilitySpec);
	return OutHandle;
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::GiveAbilityAndActivateOnce(FGameplayAbilitySpec& Spec, FGameplayEventData* GameplayEventData)
{
	static void (*GiveAbilityAndActivateOnceInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec, FGameplayEventData*) = decltype(GiveAbilityAndActivateOnceInternal)(ImageBase + Finder::FindUAbilitySystemComponent_GiveAbilityAndActivateOnce());
	FGameplayAbilitySpecHandle OutHandle;
	GiveAbilityAndActivateOnceInternal(this, &OutHandle, Spec, GameplayEventData);
	return OutHandle;
}