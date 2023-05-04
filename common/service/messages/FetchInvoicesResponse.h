#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <arrays/Enumerator.h>
#include <data/impl/InvoiceTemplate.h>
#include "../../CommonTypes.h"

class WireInvoice : public InvoiceTemplate<IInvoice>
{
public:
	WireInvoice(uint32_t invoiceId,InvoiceScope scope);
	InvoiceDirection GetInvoiceDirection() const;
	void serialize(SF::Archive &archive);
	bool GetIsLocked() const;
	void SetIsLocked(bool bLocked);	
private:
	bool m_Locked;
};

class FetchInvoicesResponse
{
private:
	NewPtrFlushArray<WireInvoice> m_Invoices;
public:
	FetchInvoicesResponse();
	~FetchInvoicesResponse();
	
	WireInvoice* AddInvoice(uint32_t invoiceId,InvoiceScope scope);
		
	PtrEnumerator<const WireInvoice> GetInvoices();
	count_t GetInvoicesCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchInvoicesResponse> FetchInvoicesResponseObj;
