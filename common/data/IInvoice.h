#ifndef __IINVOICE_H__
#define __IINVOICE_H__

#include "IIdable.h"
#include "InvoiceType.h"
#include <string>
#include "../types/DateTime.h"
#include "../types/Decimal.h"
#include "ICowInvoiceEntry.h"
#include "IInvoiceHent.h"
#include "IInvoiceDeduction.h"
#include "../arrays/Enumerator.h"
#include "PayWay.h"
#include "InvoiceDirection.h"

#define MIN_PAYDUEDAYS (1)
#define MAX_PAYDUEDAYS 365


enum InvoiceScope
{
	InvoiceScope_Cows = 'C',
	InvoiceScope_Docs = 'D',
	InvoiceScope_None = '\0'
};

class IInvoice : public IIdable
{
public:
	virtual ~IInvoice() = 0;

	virtual uint32_t GetInvoiceNo() const = 0;

	virtual uint32_t GetCorrectFor() const = 0;

	virtual InvoiceType GetInvoiceType() const = 0;

	virtual const std::wstring& GetExtras() const = 0;

	virtual const DateTime& GetInvoiceDate() const = 0;
	
	virtual const DateTime& GetPaidDate() const = 0;	
	
	//virtual Decimal GetInvoiceNetValue() const = 0;
	
	virtual const Decimal& GetVATRate() const = 0;

	virtual PayWay GetPayWay() const = 0;

	virtual const std::wstring& GetCustomNumber() const = 0;

	virtual const std::wstring& GetTransactionPlace() const = 0;

	virtual const DateTime& GetTransactionDate() const = 0;

	virtual int GetPayDueDays() const = 0;

	virtual int GetCowQty() const = 0;
	
	virtual InvoiceDirection GetInvoiceDirection() const = 0;

	virtual InvoiceScope GetInvoiceScope() const = 0;
	
	virtual ICowInvoiceEntry* GetCow(uint32_t cowId) const = 0;

	virtual ICowInvoiceEntry* GetCowAt(uint32_t index) const = 0;	

	virtual PtrEnumerator<ICowInvoiceEntry> EnumEntries() const = 0;

	virtual const IInvoiceHent* GetInvoiceHent() const = 0;
	
	virtual PtrEnumerator<IInvoiceDeduction> EnumDeductions() const = 0;
};

#endif