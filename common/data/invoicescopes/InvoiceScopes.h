#ifndef __INVOICESCOPES_H__
#define __INVOICESCOPES_H__

#include <string>
#include "../IInvoice.h"

namespace InvoiceScopes
{
	InvoiceScope FromChar(char cInvoiceScope);
	InvoiceScope FromString(const std::wstring sInvoiceScope);
	std::wstring ToString(InvoiceScope scope);
}
	


#endif