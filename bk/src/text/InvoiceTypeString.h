#ifndef __INVOICETYPESTRING_H__
#define __INVOICETYPESTRING_H__

#include <data/InvoiceType.h>
#include <string>

class InvoiceTypeString : private std::wstring
{
public:	
	InvoiceTypeString(InvoiceType type);
	const std::wstring& ToString();
};

#endif