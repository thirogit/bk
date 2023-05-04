#ifndef __IPURCHASEINVOICE_H__
#define __IPURCHASEINVOICE_H__

#include <arrays/Enumerator.h>
#include "IPurchaseCow.h"
#include "../IIdable.h"
#include "../InvoiceType.h"
#include "IPurchaseHent.h"
#include "../PayWay.h"
#include "IPurchaseInvoiceDeduction.h"

class IPurchaseInvoice 
{
public:	
	virtual const std::wstring& GetId() const = 0;
	virtual const DateTime& GetInvoiceDate() const = 0;
	virtual const DateTime& GetTransactionDate() const = 0;	
	virtual const std::wstring& GetTransactionPlace() const = 0;	
	virtual const Decimal& GetVatRate() const = 0;
	virtual PayWay GetPayWay() const = 0;
	virtual const std::wstring& GetCustomNumber() const = 0;	
	virtual int GetPayDueDays() const = 0;	
	virtual IPurchaseHent* GetInvoiceHent() const = 0;	
	virtual InvoiceType GetInvoiceType() const = 0;
	virtual int GetCowCount() const = 0;
	virtual PtrEnumerator<IPurchaseCow> EnumCows() const = 0;
	virtual PtrEnumerator<IPurchaseInvoiceDeduction> EnumDeductions() const = 0;
};
	

#endif