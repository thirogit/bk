#include "stdafx.h"
#include "SellInvoiceLocker.h"
#include <boost\foreach.hpp>
#include "../../text/InvoiceNoText.h"
#include <arrays\DeepDelete.h>

SellInvoiceLocker::SellInvoiceLocker(SeasonSession* session) : m_session(session)
{
}

void SellInvoiceLocker::CreateAttempts(const UniqueIds& invoiceIds)
{
	DeepDelete(m_attempts);
	m_attempts.clear();

	BOOST_FOREACH(uint32_t id,invoiceIds)
	{
		SellInvoiceClientObject* pInvoiceCO = m_session->GetSellInvoice(id);
		LockAttempt* attempt = new LockAttempt(id);
		attempt->SetDescriptor(InvoiceNoText(pInvoiceCO->Object()).ToString());
		m_attempts.push_back(attempt);
	}
}

void SellInvoiceLocker::Lock(uint32_t invoiceId)
{
	m_session->LockSellInvoice(invoiceId);
}

void SellInvoiceLocker::Unlock(uint32_t invoiceId)
{
	m_session->UnlockSellInvoice(invoiceId);
}