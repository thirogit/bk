#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../CowInvoiceEntryDetails.h"

class SerializableCowInvoiceEntryDetails
{
public:
	SerializableCowInvoiceEntryDetails(CowInvoiceEntryDetails& invoiceEntryDetails);	
	void serialize(SF::Archive &archive);
protected:
	Decimal& Weight();
	uint32_t& ClassId();	
	Decimal& Price();

private:
	CowInvoiceEntryDetails& m_InvoiceEntryDetails;
};
