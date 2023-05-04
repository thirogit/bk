#ifndef __BUYINVOICE_H__
#define __BUYINVOICE_H__


#include "InvoiceTemplate.h"
#include "../IBuyInvoice.h"
#include  <casajson/json.h>

class SerializableBuyInvoice;

class BuyInvoice : public InvoiceTemplate<IBuyInvoice>
{
public:
	typedef IBuyInvoice interface_type;

	BuyInvoice(uint32_t id,InvoiceScope scope);
	InvoiceDirection GetInvoiceDirection() const;
	BuyInvoice* Clone() const;

	static BuyInvoice FromJson(const json::value& value);
	json::value ToJson() const;

	friend class SerializableBuyInvoice;
};



#endif