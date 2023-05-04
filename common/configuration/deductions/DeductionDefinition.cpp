#include "stdafx.h"
#include "DeductionDefinition.h"

DeductionDefinition::DeductionDefinition(uint32_t id) : m_Id(id)
{
}

DeductionDefinition::DeductionDefinition(const IDeductionDefinition* pSrc)
{
	m_Id = pSrc->GetId();
	CopyFrom(pSrc);
}

void DeductionDefinition::CopyFrom(const IDeductionDefinition* pSrc)
{
	SetCode(pSrc->GetCode());
	SetReason(pSrc->GetReason());
	SetFraction(pSrc->GetFraction());
	SetIsEnabledByDefault(pSrc->GetIsEnabledByDefault());
}

void DeductionDefinition::SetFrom(const DeductionDetails& details)
{
	m_details.CopyFrom(details);
}

const DeductionDetails& DeductionDefinition::GetDetails() const
{
	return m_details;
}

uint32_t	DeductionDefinition::GetId() const
{
	return m_Id;
}

const std::wstring& 	DeductionDefinition::GetCode() const
{
	return m_details.GetCode();
}

const std::wstring& 	DeductionDefinition::GetReason() const
{
	return m_details.GetReason();
}

const Decimal&	DeductionDefinition::GetFraction() const
{
	return m_details.GetFraction();
}

bool DeductionDefinition::GetIsEnabledByDefault() const
{
	return m_enabledByDefault;
}

void DeductionDefinition::SetCode(const std::wstring& sCode)
{
	m_details.SetCode(sCode);
}

void DeductionDefinition::SetReason(const std::wstring& sReason)
{
	m_details.SetReason(sReason);
}

void DeductionDefinition::SetFraction(const Decimal& fraction)
{
	m_details.SetFraction(fraction);
}

void DeductionDefinition::SetIsEnabledByDefault(bool bEnabled)
{
	m_enabledByDefault = bEnabled;
}


