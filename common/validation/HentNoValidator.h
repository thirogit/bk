#pragma once

#include "../data/types/HentNo.h"

class HentNoValidator
{
public:
	static bool IsValid(const HentNo& hentNo);
private:
	static bool IsValidPL(const HentNo& hentNo);
	
};