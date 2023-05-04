#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../BuyInvoice.h"

class SerializableBuyInvoice
{
public:
	SerializableBuyInvoice(BuyInvoice* pInvoice);	
	void serialize(SF::Archive &archive);
protected:

	uint32_t& 	InvoiceNo();
	uint32_t& 	Id();
	uint32_t& 	CorrectFor();
	InvoiceType& TypeOfInvoice();
	InvoiceScope& Scope();
	InvoiceDetails& Details();
	InvoiceContent& Content();
	InvoiceHent& Hent();
	InvoiceDeductions& Deductions();
	
private:
	BuyInvoice* m_pInvoice;
};
