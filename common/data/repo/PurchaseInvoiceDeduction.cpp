#include "stdafx.h"
#include "PurchaseInvoiceDeduction.h"


PurchaseInvoiceDeduction::PurchaseInvoiceDeduction(const std::wstring& id) : m_id(id)
{

}


void PurchaseInvoiceDeduction::CopyFrom(IPurchaseInvoiceDeduction* src)
{
	m_code = src->GetCode();
	m_reason = src->GetReason();
	m_fraction = src->GetFraction();
	m_enabled = src->GetIsEnabled();
}

const std::wstring& PurchaseInvoiceDeduction::GetId() const
{
	return m_id;
}

const std::wstring& PurchaseInvoiceDeduction::GetCode() const
{
	return m_code;
}

const std::wstring& 	PurchaseInvoiceDeduction::GetReason() const
{
	return m_reason;
}

const Decimal&	PurchaseInvoiceDeduction::GetFraction() const
{
	return m_fraction;
}

bool PurchaseInvoiceDeduction::GetIsEnabled() const
{
	return m_enabled;
}

void PurchaseInvoiceDeduction::SetCode(const std::wstring& sCode)
{
	m_code = sCode;
}

void PurchaseInvoiceDeduction::SetReason(const std::wstring& sReason)
{
	m_reason = sReason;
}

void PurchaseInvoiceDeduction::SetFraction(const Decimal& fraction)
{
	m_fraction = fraction;
}

void PurchaseInvoiceDeduction::SetIsEnabled(bool bEnabled)
{
	m_enabled = bEnabled;
}


