#ifndef __BUYINVOICECOWSSELECTINTERFACE_H__
#define __BUYINVOICECOWSSELECTINTERFACE_H__

#include "InvoiceCowsSelectInterface.h"

class BuyInvoiceCowsSelectInterface : public InvoiceCowsSelectInterface
{
public:
	virtual UniqueIds GetCowsSelection();
};

#endif