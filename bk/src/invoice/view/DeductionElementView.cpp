#include "stdafx.h"
#include "DeductionElementView.h"

DeductionElementView::DeductionElementView(ItemId id) : m_id(id)
{
}

const ItemId& DeductionElementView::GetItemId() const
{
	return m_id;
}

const std::wstring& DeductionElementView::GetCode() const
{
	return m_code;
}

const std::wstring& DeductionElementView::GetReason() const
{
	return m_reason;
}

const Decimal& DeductionElementView::GetFraction() const
{
	return m_fraction;
}

bool DeductionElementView::GetIsEnabled() const
{
	return m_enabled;
}

const NullDecimal&	DeductionElementView::GetAmount() const
{
	return m_amount;
}


void DeductionElementView::SetCode(const std::wstring& sCode)
{
	m_code = sCode;
}

void DeductionElementView::SetReason(const std::wstring& sReason)
{
	m_reason = sReason;
}

void DeductionElementView::SetFraction(const Decimal&	fraction)
{
	m_fraction = fraction;
}

void DeductionElementView::SetIsEnabled(bool bEnabled)
{
	m_enabled = bEnabled;
}

void DeductionElementView::SetAmount(const NullDecimal& amount)
{
	m_amount = amount;
}
