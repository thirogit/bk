#include "stdafx.h"
#include "CowSexText.h"
#include "../utils/StringLoader.h"
#include "../resource.h"
	
CowSexText::CowSexText(const CowSex& sex) 
{
	switch(sex.getCOWSEX())
	{
		case SEX_XX:
			std::wstring::operator=(StringLoader(IDS_SEX_XX));
			break;
		case SEX_UNK:
			std::wstring::operator=(StringLoader(IDS_SEX_UNKNOWN));
			break;
		case SEX_XY:
			std::wstring::operator=(StringLoader(IDS_SEX_XY));		
			break;
	}
}
	
const std::wstring& CowSexText::ToString() const
{
	return *this;  	
}

