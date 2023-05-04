#include "stdafx.h"
#include "CowNoValidator.h"
#include <stdint.h>
#include <cctype>
#include <cwctype>

bool CowNoValidator::IsValid(const CowNo& cowNo)
{
	if(cowNo.IsNull())
		return false;

	struct VALIDATOR_FUN
	{	
		uint32_t country;
		bool (*validationFn)(const CowNo&);
	} 
	validators[] = 
	{
		{616, &CowNoValidator::IsValidPL},
		{0,NULL}
	};

	VALIDATOR_FUN* validator = validators;
	while(validator->country != 0)
	{
		if(cowNo.GetCountry().GetCountryISONumber() == validator->country)
			return validator->validationFn(cowNo);
		validator++;
	}
	return true;
}

bool CowNoValidator::IsValidPL(const CowNo& cowNo)
{
	wchar_t digit;
	std::wstring numPart = cowNo.GetNumber();
	int cksum = 0;

	if(numPart.length() != 12) return false;	

	for(int i = 0;i < 11 ;i++)
	{
		digit = numPart[i];
		if (!std::iswdigit(digit)) return false;

		cksum += (digit - 0x30) * ((i%2) ? 1 : 3);
	}
	cksum = 10 - (cksum%10);

	return ((cksum+0x30) == (int)numPart[11]);
	
}
