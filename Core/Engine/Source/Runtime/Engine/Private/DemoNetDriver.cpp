#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DemoNetDriver.h"

void UDemoNetDriver::TickFlushInternal(UDemoNetDriver* This, float DeltaSeconds) {
	if (Version::Fortnite_Version >= 3.0 && Version::Fortnite_Version <= 3.0) {
		return;
	}

	return TickFlushInternalOG(This, DeltaSeconds);
}