#pragma once

#include "../data/types/CowNo.h"

class CowNoValidator
{
public:
	static bool IsValid(const CowNo& cowNo);
private:
	static bool IsValidPL(const CowNo& cowNo);
	
};