#ifndef __SELLINVOICECOWSSELECTINTERFACE_H__
#define __SELLINVOICECOWSSELECTINTERFACE_H__

#include "InvoiceCowsSelectInterface.h"

class SellInvoiceCowsSelectInterface : public InvoiceCowsSelectInterface
{
public:
	virtual UniqueIds GetCowsSelection();
};

#endif