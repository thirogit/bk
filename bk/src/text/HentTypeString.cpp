#include "stdafx.h"
#include "HentTypeString.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

	
HentTypeString::HentTypeString(HentType hentType) 
{
	switch(hentType)
	{
		case HentType_Individual:
			std::wstring::operator=(StringLoader(IDS_HENTTYPE_INDIVIDUAL));
			break;
		case HentType_Company:
			std::wstring::operator=(StringLoader(IDS_HENTTYPE_COMPANY));
			break;
	}
}
	
const std::wstring& HentTypeString::ToString()
{
	return *this;  	
}


