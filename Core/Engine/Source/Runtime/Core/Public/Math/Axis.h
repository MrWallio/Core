#pragma once
#include "pch.h"

namespace EAxis
{
	enum Type
	{
		None,
		X,
		Y,
		Z,
	};
}

namespace EAxisList
{
	enum Type
	{
		None = 0,
		X = 1,
		Y = 2,
		Z = 4,

		Screen = 8,
		XY = X | Y,
		XZ = X | Z,
		YZ = Y | Z,
		XYZ = X | Y | Z,
		All = XYZ | Screen,
	};
}
