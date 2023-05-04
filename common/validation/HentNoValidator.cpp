#include "stdafx.h"
#include "HentNoValidator.h"
#include <stdint.h>
#include <cctype>

bool HentNoValidator::IsValid(const HentNo& hentNo)
{
	if(hentNo.IsNull())
		return false;

	struct VALIDATOR_FUN
	{	
		uint32_t country;
		bool (*validationFn)(const HentNo&);
	} 
	validators[] = 
	{
		{616, &HentNoValidator::IsValidPL},
		{0,NULL}
	};

	VALIDATOR_FUN* validator = validators;
	while(validator->country != 0)
	{
		if(hentNo.GetCountry().GetCountryISONumber() == validator->country)
			return validator->validationFn(hentNo);
		validator++;
	}
	return true;
}

bool HentNoValidator::IsValidPL(const HentNo& hentNo)
{
	wchar_t digit;
	std::wstring numPart = hentNo.GetNumber();
	int cksum = 0,even = 0,odd = 0,evencount = 0;

	if(numPart.length() != 12) return false;	

	for(int i = 0;i < 8 ;i++)
	{
		digit = numPart[i];
		if (!iswdigit(digit)) return false;
		
		digit -= 0x30;
		
		if(digit % 2)
			odd += digit;
		else
		{
			even += digit;
			evencount++;
		}		
	}
	
	cksum = 23 * even + 17 * odd + evencount;
	cksum = (cksum%7);
	return ((cksum+0x30) == (int)numPart[8]);
	
}
