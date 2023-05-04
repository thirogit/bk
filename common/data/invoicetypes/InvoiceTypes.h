#ifndef __INVOICETYPES_H__
#define __INVOICETYPES_H__

#include <string>
#include "../InvoiceType.h"

namespace InvoiceTypes
{
	InvoiceType FromChar(char cInvoiceType);
	InvoiceType FromString(const std::wstring sInvoiceType);
	std::wstring ToString(InvoiceType type);
}
	


#endif