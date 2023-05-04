#include "stdafx.h"
#include "CowEditor.h"

const Decimal& CowEditor::GetWeight() const
{
	return m_Details.GetWeight();
}
	
void  CowEditor::SetWeight(const Decimal& weight)
{
	m_Details.SetWeight(weight);
}


uint32_t CowEditor::GetStockId() const
{
	return m_Details.GetStockId();
}
	
void CowEditor::SetStockId(uint32_t stockId)
{
	m_Details.SetStockId(stockId);
}

uint32_t CowEditor::GetClassId() const
{
	return m_Details.GetClassId();
}
		
void CowEditor::SetClassId(uint32_t classId)
{
	m_Details.SetClassId(classId);
}

const CowNo& CowEditor::GetCowNo() const
{
	return m_Details.GetCowNo();
}
	
void CowEditor::SetCowNo(const CowNo& cowNo)
{
	m_Details.SetCowNo(cowNo);
}

const CowNo& CowEditor::GetMotherNo() const
{
	return m_Details.GetMotherNo();
}

void CowEditor::SetMotherNo(const CowNo& motherNo)
{
	m_Details.SetMotherNo(motherNo);
}

const DateTime&	CowEditor::GetBirthDate() const
{
	return m_Details.GetBirthDate();
}

void CowEditor::SetBirthDate(const DateTime& birthDt)
{
	m_Details.SetBirthDate(birthDt);
}

const std::wstring& CowEditor::GetPassNo() const
{
	return m_Details.GetPassNo();
}

void CowEditor::SetPassNo(const std::wstring& passNo)
{
	m_Details.SetPassNo(passNo);
}

const std::wstring& CowEditor::GetHealthCertNo() const
{
	return m_Details.GetHealthCertNo();
}

void CowEditor::SetHealthCertNo(const std::wstring& sHealthCertNo)
{
	m_Details.SetHealthCertNo(sHealthCertNo);
}

const DateTime&	CowEditor::GetPassDate() const
{
	return m_Details.GetPassDate();
}
	
void CowEditor::SetPassDate(const DateTime& passDt)
{
	m_Details.SetPassDate(passDt);
}

const std::wstring& CowEditor::GetBirthPlace() const
{
	return m_Details.GetBirthPlace();
}

void CowEditor::SetBirthPlace(const std::wstring& birthPlace)
{
	m_Details.SetBirthPlace(birthPlace);
}

const std::wstring& CowEditor::GetExtras() const
{
	return m_Details.GetExtras();
}

void CowEditor::SetExtras(const std::wstring& sExtras)
{
	m_Details.SetExtras(sExtras);
}
	
uint32_t CowEditor::GetFirstOwnerId() const
{
	return m_Details.GetFirstOwnerId();
}

void CowEditor::SetFirstOwnerId(uint32_t hentId)
{
	m_Details.SetFirstOwnerId(hentId);
}

const CowSex& CowEditor::GetSex() const
{
	return m_Details.GetSex();
}

void CowEditor::SetSex(const CowSex& cowsex)
{
	m_Details.SetSex(cowsex);
}
