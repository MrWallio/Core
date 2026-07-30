#pragma once
#include "pch.h"

#include "FortGenericRecord.h"

class UFortWorldRecord : public UFortGenericRecord {
public:
	DefineUnrealClass(UFortWorldRecord);

	DefineUProperty(int32, CurrentZoneIndex);
	DefineUProperty(FString, WUID);
};
