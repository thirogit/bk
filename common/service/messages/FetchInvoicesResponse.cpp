#include "stdafx.h"
#include "FetchInvoicesResponse.h"
#include <data/impl/serialization/SerializableInvoiceDetails.h>
#include <data/impl/serialization/SerializableInvoiceHent.h>
#include <data/impl/serialization/SerializableInvoiceContent.h>
#include <data/impl/serialization/InvoiceDeductionsSerializer.h>

WireInvoice::WireInvoice(uint32_t invoiceId,InvoiceScope scope) : InvoiceTemplate<IInvoice>(invoiceId,scope)
{
}

InvoiceDirection WireInvoice::GetInvoiceDirection() const
{
	return InvoiceDirection_None;
}

void WireInvoice::serialize(SF::Archive &archive)
{
	archive & m_Locked;
	archive & m_invoiceid;
	archive & m_invoiceno;
	archive & m_invoicescope;
	archive & m_correctfor;
	archive & m_invoicetype;

	SerializableInvoiceDetails(m_Details).serialize(archive);
	SerializableInvoiceHent(m_Hent).serialize(archive);
	SerializableInvoiceContent(m_Content).serialize(archive);
	InvoiceDeductionsSerializer(m_Deductions).serialize(archive);

}

bool WireInvoice::GetIsLocked() const
{
	return m_Locked;
}

void WireInvoice::SetIsLocked(bool bLocked)
{
	m_Locked = bLocked;
}

//-------------------------------------------------------------------------------------------------

FetchInvoicesResponse::FetchInvoicesResponse() 
{
}

FetchInvoicesResponse::~FetchInvoicesResponse()
{
	m_Invoices.FreeAndFlush();
}	

WireInvoice* FetchInvoicesResponse::AddInvoice(uint32_t invoiceId,InvoiceScope scope)
{
	WireInvoice* pNewInvoice = new WireInvoice(invoiceId,scope);
	m_Invoices.Add(pNewInvoice);
	return pNewInvoice;
}
		
PtrEnumerator<const WireInvoice> FetchInvoicesResponse::GetInvoices()
{
	return PtrEnumerator<const WireInvoice>(m_Invoices);
}

count_t FetchInvoicesResponse::GetInvoicesCount() const
{
	return m_Invoices.size();
}

void FetchInvoicesResponse::serialize(SF::Archive &archive)
{
	count_t count;

	if(archive.isRead())
	{
		archive & count;
		for(count_t i = 0;i < count;i++)
		{
			AddInvoice(NULL_ID,InvoiceScope_None)->serialize(archive);
		}
	}	
	else
	{
		count = m_Invoices.size();
		archive & count;
		for(count_t i = 0;i < count;i++)
		{
			m_Invoices[i]->serialize(archive);
		}
	}
}
