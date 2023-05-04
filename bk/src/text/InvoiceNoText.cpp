#include "stdafx.h"
#include "InvoiceNoText.h"
#include "../utils/StringLoader.h"
#include "../resource.h"
#include <boost/format.hpp>
#include "../text/InvoiceTypeString.h"

InvoiceNoText::InvoiceNoText(const IInvoice* pInvoice) 
{
	if(pInvoice)
	{
		std::wstring::operator=((boost::wformat(L"%1%%2$05d") % InvoiceTypeString(pInvoice->GetInvoiceType()).ToString() % pInvoice->GetInvoiceNo()).str());				
	}
}
	
const std::wstring& InvoiceNoText::ToString()
{
	return *this;  	
}


