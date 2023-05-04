#include "stdafx.h"
#include "PurchaseCowView.h"

PurchaseCowView::PurchaseCowView(const std::wstring& id)
{	
	m_Id = id;
}

const std::wstring& PurchaseCowView::GetId() const
{
	return m_Id;
}

const std::wstring& PurchaseCowView::GetClassCd() const
{
	return m_classCd;
}

void PurchaseCowView::SetClassCd(const std::wstring& classCd)
{
	m_classCd = classCd;
}

const Decimal& PurchaseCowView::GetWeight() const
{
	return m_weight;
}
void PurchaseCowView::SetWeight(const Decimal& weight)
{
	m_weight = weight;
}

const std::wstring& PurchaseCowView::GetStockCd()  const
{
	return m_stockCd;
}

void PurchaseCowView::SetStockCd(const std::wstring& stockCd)
{
	m_stockCd = stockCd;
}

const CowNo& PurchaseCowView::GetCowNo() const
{
	return m_cowno;
}

void PurchaseCowView::SetCowNo(const CowNo& cowNo)
{
	m_cowno = cowNo;
}

const CowNo& PurchaseCowView::GetMotherNo() const
{
	return m_motherNo;
}

void PurchaseCowView::SetMotherNo(const CowNo& motherNo)
{
	m_motherNo = motherNo;
}

const DateTime& PurchaseCowView::GetBirthDate() const
{
	return m_birthdate;
}

void PurchaseCowView::SetBirthDate(const DateTime& birthDt)
{
	m_birthdate = birthDt;
}

const std::wstring& PurchaseCowView::GetPassNo() const
{
	return m_passno;
}
void PurchaseCowView::SetPassNo(const std::wstring& passNo)
{
	m_passno = passNo;
}

const DateTime& PurchaseCowView::GetPassDate() const
{
	return m_passdate;
}

void PurchaseCowView::SetPassDate(const DateTime& passDt)
{
	m_passdate = passDt;
}

const std::wstring& PurchaseCowView::GetBirthPlace() const
{
	return m_birthplace;
}

void PurchaseCowView::SetBirthPlace(const std::wstring& birthPlace)
{
	m_birthplace = birthPlace;
}

const HentNo& PurchaseCowView::GetFirstOwnerNo() const
{
	return m_fstownerNo;
}

void PurchaseCowView::SetFirstOwnerNo(const HentNo& hentNo)
{
	m_fstownerNo = hentNo;
}

const CowSex& PurchaseCowView::GetSex() const
{
	return m_sex;
}

void PurchaseCowView::SetSex(const CowSex& cowsex)
{
	m_sex = cowsex;
}

const std::wstring&	PurchaseCowView::GetHealthCertNo() const
{
	return m_healthcertno;
}

void PurchaseCowView::SetHealthCertNo(const std::wstring& sHealthCertNo)
{
	m_healthcertno = sHealthCertNo;
}

const Decimal& PurchaseCowView::GetPrice() const
{
	return m_price;
}

void PurchaseCowView::SetPrice(const Decimal& price)
{
	m_price = price;
}
