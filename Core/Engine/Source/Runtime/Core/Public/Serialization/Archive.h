#pragma once
#include "pch.h"

class FArchive {
public:
	void** VTable;

public:
	FArchive() : VTable(nullptr) {}
};
