#include "stdafx.h"
#include "CRUDDeductionDefinition.h"

CRUDDeductionDefinition::CRUDDeductionDefinition(ItemId id, const DeductionDefinition* object) : CRUDObject(id)
{
	m_object = new DeductionDefinition(object);
}

CRUDDeductionDefinition::CRUDDeductionDefinition(ItemId id) : CRUDObject(id)
{
	m_object = new DeductionDefinition(NULL_ID);
	ChangeFlag(CRUDFlag_New);
}

const DeductionDefinition* CRUDDeductionDefinition::Obj() const
{
	return m_object;
}

CRUDDeductionDefinition::~CRUDDeductionDefinition()
{
	delete m_object;
}

const std::wstring& CRUDDeductionDefinition::GetCode() const
{
	return m_object->GetCode();
}

const std::wstring& CRUDDeductionDefinition::GetReason() const
{
	return m_object->GetReason();
}

const Decimal& CRUDDeductionDefinition::GetFraction() const
{
	return m_object->GetFraction();
}

bool CRUDDeductionDefinition::GetIsEnabled() const
{
	return m_object->GetIsEnabledByDefault();
}

void CRUDDeductionDefinition::SetCode(const std::wstring& sCode)
{
	m_object->SetCode(sCode);
}

void CRUDDeductionDefinition::SetReason(const std::wstring& sReason)
{
	m_object->SetReason(sReason);
}

void CRUDDeductionDefinition::SetFraction(const Decimal&	fraction)
{
	m_object->SetFraction(fraction);
}

void CRUDDeductionDefinition::SetIsEnabled(bool bEnabled)
{
	m_object->SetIsEnabledByDefault(bEnabled);
}

