#ifndef __INVOICEKEY_H__
#define __INVOICEKEY_H__

#include "InvoiceType.h"
#include "IInvoice.h"
#include <cstdint>

class InvoiceKey
{
public:
	InvoiceKey();
	InvoiceKey(const InvoiceKey& key);
	InvoiceKey& operator=(const InvoiceKey& right);
	
	InvoiceKey(InvoiceDirection direction,uint32_t invoiceId);
	InvoiceDirection GetDirection() const;
	uint32_t GetInvoiceId() const;
	
	void SetDirection(InvoiceDirection direction);
	void SetInvoiceId(uint32_t invoiceId);
private:
	InvoiceDirection m_direction;
	uint32_t m_invoiceId;
};



#endif