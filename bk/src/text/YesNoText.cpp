#include "stdafx.h"
#include "YesNoText.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

	
YesNoText::YesNoText(bool bYesNo) 
{
	if(bYesNo)
	{
		std::wstring::operator=(StringLoader(IDS_YES));
	}
	else
	{
		std::wstring::operator=(StringLoader(IDS_NO));	
	}
}
	
const std::wstring& YesNoText::ToString()
{
	return *this;  	
}


