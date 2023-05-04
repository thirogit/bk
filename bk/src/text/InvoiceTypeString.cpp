#include "stdafx.h"
#include "InvoiceTypeString.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

	
InvoiceTypeString::InvoiceTypeString(InvoiceType type) 
{
	switch(type)
	{
		case InvoiceType_Regular:
			std::wstring::operator=(StringLoader(IDS_INVOICETYPE_REGULAR));
			break;
		case InvoiceType_Lump:
			std::wstring::operator=(StringLoader(IDS_INVOICETYPE_LUMP));
			break;
	}
}
	
const std::wstring& InvoiceTypeString::ToString()
{
	return *this;  	
}


