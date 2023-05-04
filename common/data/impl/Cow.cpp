#include "stdafx.h"
#include "Cow.h"

Cow::Cow(int Id/* = NULL_ID*/)
{
	m_cowId = Id;	
	m_buyInvoice = NULL_ID;
	m_sellInvoice = NULL_ID;
}

Cow::~Cow()
{	
}


uint32_t Cow::GetClassId() const
{
	return m_details.GetClassId();
}

void Cow::SetClassId(uint32_t classId)
{
	m_details.SetClassId(classId);
}

const Decimal& Cow::GetWeight() const
{
	return m_details.GetWeight();
}
void Cow::SetWeight(const Decimal& weight)
{
	m_details.SetWeight(weight);
}

uint32_t Cow::GetStockId()  const
{
	return m_details.GetStockId();
}

void Cow::SetStockId(uint32_t stockId)
{
	m_details.SetStockId(stockId);
}

const CowNo& Cow::GetCowNo() const
{
	return m_details.GetCowNo();
}

void Cow::SetCowNo(const CowNo& cowNo)
{
	m_details.SetCowNo(cowNo);
}

const CowNo& Cow::GetMotherNo() const
{
	return m_details.GetMotherNo();
}

void Cow::SetMotherNo(const CowNo& motherNo)
{
	m_details.SetMotherNo(motherNo);
}

const DateTime& Cow::GetBirthDate() const
{
	return m_details.GetBirthDate();
}

void Cow::SetBirthDate(const DateTime& birthDt)
{
	m_details.SetBirthDate(birthDt);
}

const std::wstring& Cow::GetPassNo() const
{
	return m_details.GetPassNo();
}
void Cow::SetPassNo(const std::wstring& passNo)
{
	m_details.SetPassNo(passNo);
}

const DateTime& Cow::GetPassDate() const
{
	return m_details.GetPassDate();
}

void Cow::SetPassDate(const DateTime& passDt)
{
	m_details.SetPassDate(passDt);
}

const std::wstring& Cow::GetBirthPlace() const
{
	return m_details.GetBirthPlace();
}

void Cow::SetBirthPlace(const std::wstring& birthPlace)
{
	m_details.SetBirthPlace(birthPlace);
}

const std::wstring& Cow::GetExtras() const
{
	return m_details.GetExtras();
}

void Cow::SetExtras(const std::wstring& sExtras)
{
	m_details.SetExtras(sExtras);
}

uint32_t Cow::GetFirstOwnerId() const
{
	return m_details.GetFirstOwnerId();
}

void Cow::SetFirstOwnerId(uint32_t hentId)
{
	m_details.SetFirstOwnerId(hentId);
}

const CowSex& Cow::GetSex() const
{
	return m_details.GetSex();
}

void Cow::SetSex(const CowSex& cowsex)
{
	m_details.SetSex(cowsex);
}

uint32_t Cow::GetBuyInvoiceId() const
{
	return m_buyInvoice;
}

void Cow::SetBuyInvoiceId(uint32_t invoiceId)
{
	m_buyInvoice = invoiceId;
}

uint32_t Cow::GetSellInvoiceId() const
{
	return m_sellInvoice;
}

void Cow::SetSellInvoiceId(uint32_t invoiceId)
{
	m_sellInvoice = invoiceId;
}

const NullDecimal& Cow::GetTermBuyPrice() const
{
	return m_details.GetTermBuyPrice();
}

void Cow::SetTermBuyPrice(const NullDecimal& dTermBuyPrice) 
{
	m_details.SetTermBuyPrice(dTermBuyPrice);
}

const NullDecimal& Cow::GetTermSellPrice() const
{
	return m_details.GetTermSellPrice();
}

void Cow::SetTermSellPrice(const NullDecimal& dTermSellPrice) 
{
	m_details.SetTermSellPrice(dTermSellPrice);
}

const NullDecimal& Cow::GetTermBuyWeight() const
{
	return m_details.GetTermBuyWeight();
}
void Cow::SetTermBuyWeight(const NullDecimal& buyWeight)
{
	m_details.SetTermBuyWeight(buyWeight);
}

const NullDecimal& Cow::GetTermSellWeight()const
{
	return m_details.GetTermSellWeight();
}

void Cow::SetTermSellWeight(const NullDecimal& sellWeight)
{
	m_details.SetTermSellWeight(sellWeight);
}

uint32_t Cow::GetTermBuyClassId() const
{
	return m_details.GetTermBuyClassId();
}

void Cow::SetTermBuyClassId(uint32_t pBuyStock)
{
	m_details.SetTermBuyClassId(pBuyStock);
}

uint32_t Cow::GetTermSellClassId() const
{
	return m_details.GetTermSellClassId();
}

void Cow::SetTermSellClassId(uint32_t pSellStock)
{
	m_details.SetTermSellClassId(pSellStock);
}

const std::wstring& Cow::GetPassImgLabel() const
{
	return m_passimglabel;
}

void Cow::SetPassImgLabel(const std::wstring& sPassImgLabel)
{
	m_passimglabel = sPassImgLabel;
}


const std::wstring&	Cow::GetHealthCertNo() const
{
	return m_details.GetHealthCertNo();
}

void Cow::SetHealthCertNo(const std::wstring& sHealthCertNo)
{
	m_details.SetHealthCertNo(sHealthCertNo);
}

uint32_t Cow::GetId() const
{
	return m_cowId;
}


void Cow::CopyFrom(const ICow* pSrc)
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
	SetSellInvoiceId(pSrc->GetSellInvoiceId());
	SetBuyInvoiceId(pSrc->GetBuyInvoiceId());
	SetFirstOwnerId(pSrc->GetFirstOwnerId());	
	SetSex(pSrc->GetSex());	
	SetTermBuyPrice(pSrc->GetTermBuyPrice()) ;
	SetTermSellPrice(pSrc->GetTermSellPrice()) ;
	SetTermBuyWeight(pSrc->GetTermBuyWeight()) ;
	SetTermSellWeight(pSrc->GetTermSellWeight());
	SetTermBuyClassId(pSrc->GetTermBuyClassId()) ;
	SetTermSellClassId(pSrc->GetTermSellClassId()) ;
	SetPassImgLabel(pSrc->GetPassImgLabel());

}


void Cow::SetFrom(const CowDetails& details)
{
	m_details.CopyFrom(details);
}

const CowDetails& Cow::GetDetails() const
{
	return m_details;
}
