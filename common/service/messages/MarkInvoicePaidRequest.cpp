#include "stdafx.h"
#include "MarkInvoicePaidRequest.h"
#include <data/types/serialization/TypesSerialization.h>

MarkInvoicePaidRequest::MarkInvoicePaidRequest(uint32_t invoiceId/* = NULL_ID*/) : m_invoiceId(invoiceId)
{
}

MarkInvoicePaidRequest& MarkInvoicePaidRequest::WithPaidDate(const DateTime& paidDt)
{
	m_paidDt = paidDt;
	return *this;
}

const DateTime& MarkInvoicePaidRequest::GetPaidDt() const
{
	return m_paidDt;
}

uint32_t MarkInvoicePaidRequest::GetInvoiceId() const
{
	return m_invoiceId;
}

void MarkInvoicePaidRequest::serialize(SF::Archive &archive)
{
	archive & m_invoiceId;
	archive & m_paidDt;
}

