#ifndef __ICOWINVOICEENTRY_H__
#define __ICOWINVOICEENTRY_H__

#include "../types/Decimal.h"

class ICowInvoiceEntry
{
public:	
	virtual uint32_t		GetCowId() const = 0;
	virtual uint32_t		GetClassId() const = 0;
	virtual const Decimal&	GetWeight() const = 0;
	virtual const Decimal&	GetPrice() const = 0;
};

#endif