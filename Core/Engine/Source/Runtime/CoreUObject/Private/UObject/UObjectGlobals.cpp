#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

void BeginLoad(FUObjectSerializeContext* LoadContext, const TCHAR* DebugContext) {
	void (*BeginLoadInternal)(FUObjectSerializeContext*, const TCHAR*) = decltype(BeginLoadInternal)(ImageBase + Finder::FindBeginLoad());
	BeginLoadInternal(LoadContext, DebugContext);
}

void EndLoad(FUObjectSerializeContext* LoadContext) {
	void (*EndLoadInternal)(FUObjectSerializeContext*) = decltype(EndLoadInternal)(ImageBase + Finder::FindEndLoad());
	EndLoadInternal(LoadContext);
}