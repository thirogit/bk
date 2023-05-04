#ifndef __INVOICENOTEXT_H__
#define __INVOICENOTEXT_H__

#include <string>
#include <data\IInvoice.h>

class InvoiceNoText : private std::wstring
{
public:	
	InvoiceNoText(const IInvoice* pInvoice);	
	const std::wstring& ToString();
};

#endif