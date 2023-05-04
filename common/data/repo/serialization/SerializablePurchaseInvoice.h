#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../PurchaseInvoice.h"

class SerializablePurchaseInvoice
{
public:
	SerializablePurchaseInvoice(PurchaseInvoice* pInvoice);
	void serialize(SF::Archive &archive);
protected:
	std::wstring& Id();
	DateTime& InvoiceDate();
	DateTime& TransactionDate();
	std::wstring& TransactionPlace();
	Decimal& VatRate();
	PayWay& PaymentWay();
	std::wstring& CustomNumber();
	int& PayDueDays();
	InvoiceType& TypeOfInvoice();
	
private:
	PurchaseInvoice* m_pInvoice;
};
