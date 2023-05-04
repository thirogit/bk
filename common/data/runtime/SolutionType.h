#pragma once

#include <stdint.h>

enum SolutionType : uint32_t
{
	SolutionType_None = 0,
	SolutionType_Add = 1,
	SolutionType_Clear = 2,
	SolutionType_Replace = 4
};

