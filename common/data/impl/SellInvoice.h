#ifndef __SELLINVOICE_H__
#define __SELLINVOICE_H__


#include "InvoiceTemplate.h"
#include "../ISellInvoice.h"
#include  <casajson/json.h>

class SerializableSellInvoice;

class SellInvoice : public InvoiceTemplate<ISellInvoice>
{
public:
	typedef ISellInvoice interface_type;

	SellInvoice(uint32_t id,InvoiceScope scope);
	InvoiceDirection GetInvoiceDirection() const;
	SellInvoice* Clone() const;

	static SellInvoice FromJson(const json::value& value);
	json::value ToJson() const;

	friend class SerializableSellInvoice;
};



#endif