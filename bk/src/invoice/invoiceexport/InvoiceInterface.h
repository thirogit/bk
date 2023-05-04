#ifndef __INVOICEINTERFACE_H__
#define __INVOICEINTERFACE_H__

#include "../../runtime/UniqueIds.h"
#include <types\DateRange.h>
#include <data\PayWay.h>
#include <data\InvoiceType.h>
#include <data\IInvoice.h>
#include <data\InvoiceKind.h>

class InvoiceInterface
{	
public:
	virtual UniqueIds Search(const DateRange& range,PayWay payway) = 0;
	virtual InvoiceKind GetInvoiceKind() const = 0;
	virtual IInvoice* GetInvoice(uint32_t id) = 0;
};

#endif
