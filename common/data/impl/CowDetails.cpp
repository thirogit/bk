#include "stdafx.h"
#include "CowDetails.h"


CowDetails::CowDetails()
{	
	m_stockId = NULL_ID;	
	m_classId = NULL_ID;	
	m_sex;	
	m_fstownerId = NULL_ID;	
}

CowDetails::CowDetails(const CowDetails& src)
{
	CopyFrom(src);
}

uint32_t CowDetails::GetClassId() const
{
	return m_classId;
}

void CowDetails::SetClassId(uint32_t classId)
{
	m_classId = classId;
}

const Decimal& CowDetails::GetWeight() const
{
	return m_weight;
}
void CowDetails::SetWeight(const Decimal& weight)
{
	m_weight = weight;
}

uint32_t CowDetails::GetStockId()  const
{
	return m_stockId;
}

void CowDetails::SetStockId(uint32_t stockId)
{
	m_stockId = stockId;
}

const CowNo& CowDetails::GetCowNo() const
{
	return m_cowno;
}

void CowDetails::SetCowNo(const CowNo& cowNo)
{
	m_cowno = cowNo;
}

const CowNo& CowDetails::GetMotherNo() const
{
	return m_motherNo;
}

void CowDetails::SetMotherNo(const CowNo& motherNo)
{
	m_motherNo = motherNo;
}

const DateTime& CowDetails::GetBirthDate() const
{
	return m_birthdate;
}

void CowDetails::SetBirthDate(const DateTime& birthDt)
{
	m_birthdate = birthDt;
}

const std::wstring& CowDetails::GetPassNo() const
{
	return m_passno;
}
void CowDetails::SetPassNo(const std::wstring& passNo)
{
	m_passno = passNo;
}

const DateTime& CowDetails::GetPassDate() const
{
	return m_passdate;
}

void CowDetails::SetPassDate(const DateTime& passDt)
{
	m_passdate = passDt;
}

const std::wstring& CowDetails::GetBirthPlace() const
{
	return m_birthplace;
}

void CowDetails::SetBirthPlace(const std::wstring& birthPlace)
{
	m_birthplace = birthPlace;
}

const std::wstring& CowDetails::GetExtras() const
{
	return m_extras;
}

void CowDetails::SetExtras(const std::wstring& sExtras)
{
	m_extras = sExtras;
}

uint32_t CowDetails::GetFirstOwnerId() const
{
	return m_fstownerId;
}

void CowDetails::SetFirstOwnerId(uint32_t hentId)
{
	m_fstownerId = hentId;
}

const CowSex& CowDetails::GetSex() const
{
	return m_sex;
}

void CowDetails::SetSex(const CowSex& cowsex)
{
	m_sex = cowsex;
}

const std::wstring&	CowDetails::GetHealthCertNo() const
{
	return m_healthcertno;
}

void CowDetails::SetHealthCertNo(const std::wstring& sHealthCertNo)
{
	m_healthcertno = sHealthCertNo;
}

const NullDecimal& CowDetails::GetTermBuyPrice() const
{
	return m_termbuyprice;
}

void CowDetails::SetTermBuyPrice(const NullDecimal& dTermBuyPrice)
{
	m_termbuyprice = dTermBuyPrice;
}

const NullDecimal&	CowDetails::GetTermSellPrice() const
{
	return m_termsellprice;
}

void CowDetails::SetTermSellPrice(const NullDecimal& dTermSellPrice)
{
	m_termsellprice = dTermSellPrice;
}

const NullDecimal&	CowDetails::GetTermBuyWeight() const
{
	return m_termbuyweight;
}

void CowDetails::SetTermBuyWeight(const NullDecimal& buyWeight)
{
	m_termbuyweight = buyWeight;
}

const NullDecimal&	CowDetails::GetTermSellWeight() const
{
	return m_termsellweight;
}

void CowDetails::SetTermSellWeight(const NullDecimal& sellWeight)
{
	m_termsellweight = sellWeight;
}

uint32_t CowDetails::GetTermBuyClassId() const
{
	return m_termbuyclassId;
}

void CowDetails::SetTermBuyClassId(uint32_t classId)
{
	m_termbuyclassId = classId;
}

uint32_t CowDetails::GetTermSellClassId() const
{
	return m_termsellclassId;
}

void CowDetails::SetTermSellClassId(uint32_t classId)
{
	m_termsellclassId = classId;
}


void CowDetails::CopyFrom(const CowDetails& src)
{
	m_weight = src.m_weight;
	m_stockId = src.m_stockId;	
	m_classId = src.m_classId;	
	m_cowno = src.m_cowno;
	m_birthdate = src.m_birthdate;
	m_birthplace = src.m_birthplace;
	m_sex = src.m_sex;
	m_motherNo = src.m_motherNo;	
	m_passno = src.m_passno;
	m_passdate = src.m_passdate;
	m_fstownerId = src.m_fstownerId;
	m_extras = src.m_extras;		
	m_healthcertno = src.m_healthcertno;
	m_termbuyclassId = src.m_termbuyclassId;
	m_termbuyweight = src.m_termbuyweight;
	m_termsellclassId = src.m_termsellclassId;
	m_termsellweight = src.m_termsellweight;
	m_termbuyprice = src.m_termbuyprice;
	m_termsellprice = src.m_termsellprice;
}

void CowDetails::CopyFrom(const ICow* pSrc)
{		
	SetWeight(pSrc->GetWeight());	
	SetStockId(pSrc->GetStockId());		
	SetClassId(pSrc->GetClassId());	
	SetCowNo(pSrc->GetCowNo());
	SetMotherNo(pSrc->GetMotherNo());
	SetBirthDate(pSrc->GetBirthDate());
	SetPassNo(pSrc->GetPassNo());
	SetHealthCertNo(pSrc->GetHealthCertNo());
	SetPassDate(pSrc->GetPassDate());
	SetBirthPlace(pSrc->GetBirthPlace());
	SetExtras(pSrc->GetExtras());
	SetFirstOwnerId(pSrc->GetFirstOwnerId());
	SetSex(pSrc->GetSex());	
	SetTermBuyClassId(pSrc->GetTermBuyClassId());
	SetTermBuyWeight(pSrc->GetTermBuyWeight());
	SetTermSellWeight(pSrc->GetTermSellWeight());
	SetTermSellClassId(pSrc->GetTermSellClassId());
	SetTermBuyPrice(pSrc->GetTermBuyPrice());
	SetTermSellPrice(pSrc->GetTermSellPrice());
}

