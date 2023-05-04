#ifndef __INVOICETYPES_H__
#define __INVOICETYPES_H__

#include <string>
#include "../InvoiceType.h"

namespace InvoiceTypes
{
	InvoiceType GetInvoiceType(char cInvoiceType);
	InvoiceType GetInvoiceType(const std::wstring sInvoiceType);
}
	


#endif