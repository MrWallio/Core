#include "pch.h"
#include "../Public/GameplayAbilitySpecHandle.h"

void FGameplayAbilitySpecHandle::GenerateNewHandle()
{
	if (Finder::FindGHandle()) {
		static int32 GHandle = *(int32*)(Finder::FindGHandle() + ImageBase);
		Handle = GHandle++;
	}
	else {
		Handle = rand();
	}
}