#include "stdafx.h"
#include "DeductionDefinitionView.h"

DeductionDefinitionView::DeductionDefinitionView(ItemId id) : m_id(id)
{
}

const ItemId& DeductionDefinitionView::GetItemId() const
{
	return m_id;
}

const std::wstring& DeductionDefinitionView::GetCode() const
{
	return m_code;
}

const std::wstring& DeductionDefinitionView::GetReason() const
{
	return m_reason;
}

const Decimal& DeductionDefinitionView::GetFraction() const
{
	return m_fraction;
}

bool DeductionDefinitionView::GetIsEnabled() const
{
	return m_enabled;
}

void DeductionDefinitionView::SetCode(const std::wstring& sCode)
{
	m_code = sCode;
}

void DeductionDefinitionView::SetReason(const std::wstring& sReason)
{
	m_reason = sReason;
}

void DeductionDefinitionView::SetFraction(const Decimal&	fraction)
{
	m_fraction = fraction;
}

void DeductionDefinitionView::SetIsEnabled(bool bEnabled)
{
	m_enabled = bEnabled;
}

