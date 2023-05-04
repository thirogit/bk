#include "stdafx.h"
#include "PurchaseInvoiceDeductionView.h"

PurchaseInvoiceDeductionView::PurchaseInvoiceDeductionView(const std::wstring& id) : m_id(id), m_enabled(false)
{
}

const std::wstring& PurchaseInvoiceDeductionView::GetId() const
{
	return m_id;
}

const std::wstring& PurchaseInvoiceDeductionView::GetCode() const
{
	return m_code;
}

const std::wstring& PurchaseInvoiceDeductionView::GetReason() const
{
	return m_reason;
}

const Decimal& PurchaseInvoiceDeductionView::GetFraction() const
{
	return m_fraction;
}

bool PurchaseInvoiceDeductionView::GetIsEnabled() const
{
	return m_enabled;
}

void PurchaseInvoiceDeductionView::SetCode(const std::wstring& sCode)
{
	m_code = sCode;
}

void PurchaseInvoiceDeductionView::SetReason(const std::wstring& sReason)
{
	m_reason = sReason;
}

void PurchaseInvoiceDeductionView::SetFraction(const Decimal&	fraction)
{
	m_fraction = fraction;
}

void PurchaseInvoiceDeductionView::SetIsEnabled(bool bEnabled)
{
	m_enabled = bEnabled;
}