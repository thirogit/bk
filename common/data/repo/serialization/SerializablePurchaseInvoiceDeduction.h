#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../PurchaseInvoiceDeduction.h"

class SerializablePurchaseInvoiceDeduction
{
public:
	SerializablePurchaseInvoiceDeduction(PurchaseInvoiceDeduction* pInvoiceDeduction);
	void serialize(SF::Archive &archive);
protected:
	std::wstring& Id();
	std::wstring& Code();	
	std::wstring& Reason();	
	Decimal& Fraction();	
	bool& IsEnabled();
	
private:
	PurchaseInvoiceDeduction* m_pDeduction;
};
