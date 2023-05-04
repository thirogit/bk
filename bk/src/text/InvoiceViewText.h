#ifndef __INVOICEVIEWTEXT_H__
#define __INVOICEVIEWTEXT_H__

#include "../view/InvoiceView.h"

class InvoiceViewText 
{
public:
	virtual std::wstring Text(const InvoiceView *pView,uint32_t memberId);

};

#endif