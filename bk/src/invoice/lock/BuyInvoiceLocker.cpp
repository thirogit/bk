#include "stdafx.h"
#include "BuyInvoiceLocker.h"
#include <boost\foreach.hpp>
#include "../../text/InvoiceNoText.h"
#include <arrays\DeepDelete.h>

BuyInvoiceLocker::BuyInvoiceLocker(SeasonSession* session) : m_session(session)
{
}

void BuyInvoiceLocker::CreateAttempts(const UniqueIds& invoiceIds)
{
	DeepDelete(m_attempts);
	m_attempts.clear();

	BOOST_FOREACH(uint32_t id,invoiceIds)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = m_session->GetBuyInvoice(id);
		LockAttempt* attempt = new LockAttempt(id);
		attempt->SetDescriptor(InvoiceNoText(pBuyInvoiceCO->Object()).ToString());
		m_attempts.push_back(attempt);
	}
}

void BuyInvoiceLocker::Lock(uint32_t invoiceId)
{
	m_session->LockBuyInvoice(invoiceId);
}

void BuyInvoiceLocker::Unlock(uint32_t invoiceId)
{
	m_session->UnlockBuyInvoice(invoiceId);
}