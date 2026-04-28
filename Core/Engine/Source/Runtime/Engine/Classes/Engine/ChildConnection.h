#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

class UChildConnection : public UNetConnection
{
	DefineUnrealClass(UChildConnection);

	DefineUProperty(UNetConnection*, Parent);
};