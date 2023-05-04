#include "stdafx.h"
#include "CowView.h"

CowView::CowView(uint32_t id)
{	
	m_Id = id;
}

uint32_t CowView::GetId() const
{
	return m_Id;
}

const std::wstring& CowView::GetClassCd() const
{
	return m_classCd;
}

void CowView::SetClassCd(const std::wstring& classCd)
{
	m_classCd = classCd;
}

const Decimal& CowView::GetWeight() const
{
	return m_weight;
}
void CowView::SetWeight(const Decimal& weight)
{
	m_weight = weight;
}

const std::wstring& CowView::GetStockCd()  const
{
	return m_stockCd;
}

void CowView::SetStockCd(const std::wstring& stockCd)
{
	m_stockCd = stockCd;
}

const CowNo& CowView::GetCowNo() const
{
	return m_cowno;
}

void CowView::SetCowNo(const CowNo& cowNo)
{
	m_cowno = cowNo;
}

const CowNo& CowView::GetMotherNo() const
{
	return m_motherNo;
}

void CowView::SetMotherNo(const CowNo& motherNo)
{
	m_motherNo = motherNo;
}

const DateTime& CowView::GetBirthDate() const
{
	return m_birthdate;
}

void CowView::SetBirthDate(const DateTime& birthDt)
{
	m_birthdate = birthDt;
}

const std::wstring& CowView::GetPassNo() const
{
	return m_passno;
}
void CowView::SetPassNo(const std::wstring& passNo)
{
	m_passno = passNo;
}

const DateTime& CowView::GetPassDate() const
{
	return m_passdate;
}

void CowView::SetPassDate(const DateTime& passDt)
{
	m_passdate = passDt;
}

const std::wstring& CowView::GetBirthPlace() const
{
	return m_birthplace;
}

void CowView::SetBirthPlace(const std::wstring& birthPlace)
{
	m_birthplace = birthPlace;
}

const std::wstring& CowView::GetExtras() const
{
	return m_extras;
}

void CowView::SetExtras(const std::wstring& sExtras)
{
	m_extras = sExtras;
}

const std::wstring& CowView::GetFirstOwnerAlias() const
{
	return m_fstownerAlias;
}

void CowView::SetFirstOwnerAlias(const std::wstring& hentAlias)
{
	m_fstownerAlias = hentAlias;
}

const CowSex& CowView::GetSex() const
{
	return m_sex;
}

void CowView::SetSex(const CowSex& cowsex)
{
	m_sex = cowsex;
}

const std::wstring&	CowView::GetHealthCertNo() const
{
	return m_healthcertno;
}

void CowView::SetHealthCertNo(const std::wstring& sHealthCertNo)
{
	m_healthcertno = sHealthCertNo;
}

const NullDecimal& CowView::GetTermBuyPrice() const
{
	return m_termbuyprice;
}

void CowView::SetTermBuyPrice(const NullDecimal& dTermBuyPrice)
{
	m_termbuyprice = dTermBuyPrice;
}

const NullDecimal&	CowView::GetTermSellPrice() const
{
	return m_termsellprice;
}

void CowView::SetTermSellPrice(const NullDecimal& dTermSellPrice)
{
	m_termsellprice = dTermSellPrice;
}

const NullDecimal&	CowView::GetTermBuyWeight() const
{
	return m_termbuyweight;
}

void CowView::SetTermBuyWeight(const NullDecimal& buyWeight)
{
	m_termbuyweight = buyWeight;
}

const NullDecimal&	CowView::GetTermSellWeight() const
{
	return m_termsellweight;
}

void CowView::SetTermSellWeight(const NullDecimal& sellWeight)
{
	m_termsellweight = sellWeight;
}

const std::wstring& CowView::GetTermBuyClassCd() const
{
	return m_termbuyclassCd;
}

void CowView::SetTermBuyClassCd(const std::wstring& classCd)
{
	m_termbuyclassCd = classCd;
}

const std::wstring& CowView::GetTermSellClassCd() const
{
	return m_termsellclassCd;
}

void CowView::SetTermSellClassCd(const std::wstring& classCd)
{
	m_termsellclassCd = classCd;
}


