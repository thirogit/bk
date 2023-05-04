#include "stdafx.h"
#include "CowInvoiceEntryView.h"

CowInvoiceEntryView::CowInvoiceEntryView(uint32_t id)
{	
	m_cowId = id;
}

uint32_t CowInvoiceEntryView::GetId() const
{
	return m_cowId;
}

const std::string& CowInvoiceEntryView::GetOwnClassCd() const
{
	return m_ownClassCd;
}

void CowInvoiceEntryView::SetOwnClassCd(const std::string& classCd)
{
	m_ownClassCd = classCd;
}

const Decimal& CowInvoiceEntryView::GetOwnWeight() const
{
	return m_ownWeight;
}
void CowInvoiceEntryView::SetOwnWeight(const Decimal& weight)
{
	m_ownWeight = weight;
}

const std::string& CowInvoiceEntryView::GetOwnStockCd()  const
{
	return m_ownStockCd;
}

void CowInvoiceEntryView::SetOwnStockCd(const std::string& stockCd)
{
	m_ownStockCd = stockCd;
}

const CowNo& CowInvoiceEntryView::GetCowNo() const
{
	return m_cowNo;
}

void CowInvoiceEntryView::SetCowNo(const CowNo& cowNo)
{
	m_cowNo = cowNo;
}

const CowNo& CowInvoiceEntryView::GetMotherNo() const
{
	return m_motherNo;
}

void CowInvoiceEntryView::SetMotherNo(const CowNo& motherNo)
{
	m_motherNo = motherNo;
}

const DateTime& CowInvoiceEntryView::GetBirthDate() const
{
	return m_birthdate;
}

void CowInvoiceEntryView::SetBirthDate(const DateTime& birthDt)
{
	m_birthdate = birthDt;
}

const std::string& CowInvoiceEntryView::GetPassNo() const
{
	return m_passno;
}
void CowInvoiceEntryView::SetPassNo(const std::string& passNo)
{
	m_passno = passNo;
}

const DateTime& CowInvoiceEntryView::GetPassDate() const
{
	return m_passdate;
}

void CowInvoiceEntryView::SetPassDate(const DateTime& passDt)
{
	m_passdate = passDt;
}

const std::string& CowInvoiceEntryView::GetBirthPlace() const
{
	return m_birthplace;
}

void CowInvoiceEntryView::SetBirthPlace(const std::string& birthPlace)
{
	m_birthplace = birthPlace;
}

const CowSex& CowInvoiceEntryView::GetSex() const
{
	return m_sex;
}

void CowInvoiceEntryView::SetSex(const CowSex& cowsex)
{
	m_sex = cowsex;
}

const std::string&	CowInvoiceEntryView::GetHealthCertNo() const
{
	return m_healthcertno;
}

void CowInvoiceEntryView::SetHealthCertNo(const std::string& sHealthCertNo)
{
	m_healthcertno = sHealthCertNo;
}



