#include "stdafx.h"
#include "MotiveString.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

	
MotiveString::MotiveString(Motive motive) 
{
	switch(motive)
	{
		case Motive_Birth:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_BIRTH));
			break;
		case Motive_Buy:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_BUY));
			break;
		case Motive_Sell:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_SELL));
			break;
		case Motive_Import:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_IMPORT));
			break;
		case Motive_Export:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_EXPORT));
			break;
		case Motive_Death:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_DEATH));
			break;
		case Motive_Kill:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_KILL));
			break;
		case Motive_Slaugher:
			std::wstring::operator=(StringLoader(IDS_MOTIVE_SLAUGHER));
			break;
	}	
}
	
const std::wstring& MotiveString::ToString()
{
	return *this;  	
}


