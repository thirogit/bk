#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../InvoiceDetails.h"

class SerializableInvoiceDetails
{
public:
	SerializableInvoiceDetails(InvoiceDetails& details);	
	void serialize(SF::Archive &archive);
protected:
	
	std::wstring& Extras();
	DateTime& 	InvoiceDate();
	DateTime& 	PaidDate();
	Decimal&  	VATRate();
	PayWay& 	PaymentWay();	
	std::wstring& CustomNumber();
	int& 		PayDueDays();
	std::wstring& TransactionPlace();
	DateTime& 	TransactionDate();	
	
	
private:
	InvoiceDetails& m_Details;
};
