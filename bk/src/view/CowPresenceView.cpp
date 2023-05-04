#include "stdafx.h"
#include "CowPresenceView.h"

CowPresenceView::CowPresenceView(const CowPresenceKey& key) : m_Key(key),m_FirstOwner(NULL),m_BuyFromHent(NULL),m_SellToHent(NULL),
															  m_herdInMotive(Motive_None),m_herdOutMotive(Motive_None), m_BuyDocHent(NULL), m_SellDocHent(NULL)
 { 
 }

 CowPresenceView::CowPresenceView(const CowPresenceView& src) : m_Key(src.m_Key),m_FirstOwner(NULL),m_BuyFromHent(NULL),m_SellToHent(NULL), m_BuyDocHent(NULL), m_SellDocHent(NULL)
 {
	m_Locked = src.m_Locked;	
	m_InStock = src.m_InStock;	
	m_CowNo = src.m_CowNo;
	m_PassNo = src.m_PassNo;

	if(src.m_FirstOwner)
		m_FirstOwner = new HentView(*src.m_FirstOwner);

	m_BirthDate = src.m_BirthDate;
	m_Weight = src.m_Weight;
	m_Sex = src.m_Sex;
	m_OwnStockCd = src.m_OwnStockCd;
	m_OwnClassCd = src.m_OwnClassCd;
	m_Extras = src.m_Extras;

	if(src.m_BuyFromHent)
		m_BuyFromHent = new HentView(*src.m_BuyFromHent);

	m_BuyStockCd = src.m_BuyStockCd;
	m_BuyPrice = src.m_BuyPrice;
	m_BuyWeight = src.m_BuyWeight;
	m_HerdInDocNo = src.m_HerdInDocNo;
	m_HerdInDate = src.m_HerdInDate;
	m_HerdInGrp = src.m_HerdInGrp;
	m_HerdInClassCd = src.m_HerdInClassCd;
	m_HerdInWeight = src.m_HerdInWeight;
	m_HerdInPlateNo = src.m_HerdInPlateNo;

	if(src.m_SellToHent)
		m_SellToHent = new HentView(*src.m_SellToHent);

	m_SellStockCd = src.m_SellStockCd;
	m_SellPrice = src.m_SellPrice;
	m_SellWeight = src.m_SellWeight;
	m_HerdOutDocNo = src.m_HerdOutDocNo;
	m_HerdOutDate = src.m_HerdOutDate;
	m_HerdOutGrp = src.m_HerdOutGrp;
	m_HerdOutClassCd = src.m_HerdOutClassCd;
	m_HerdOutWeight = src.m_HerdOutWeight;
	m_HerdOutPlateNo = src.m_HerdOutPlateNo;
	m_BuyInvoiceNo = src.m_BuyInvoiceNo;
	m_SellInvoiceNo = src.m_SellInvoiceNo;
	m_TermBuyClassCd = src.m_TermBuyClassCd;
	m_TermSellClassCd = src.m_TermSellClassCd;
	m_TermBuyWeight = src.m_TermBuyWeight;
	m_TermSellWeight = src.m_TermSellWeight;
	m_TermBuyPrice = src.m_TermBuyPrice;
	m_TermSellPrice = src.m_TermSellPrice;
	m_healthCertNo = src.m_healthCertNo;
	m_motherNo = src.m_motherNo;
	m_herdInMotive  = src.m_herdInMotive;
	m_herdOutMotive  = src.m_herdOutMotive;

	if (src.m_BuyDocHent)
		m_BuyDocHent = new HentView(*src.m_BuyDocHent);

	if (src.m_SellDocHent)
		m_SellDocHent = new HentView(*src.m_SellDocHent);
	
 }

 CowPresenceView::~CowPresenceView()
 {
	 if(m_SellToHent)
		 delete m_SellToHent;

	 if(m_FirstOwner)
		 delete m_FirstOwner;

	 if(m_BuyFromHent)
		 delete m_BuyFromHent;

	 if (m_BuyDocHent)
		 delete m_BuyDocHent;

	 if (m_SellDocHent)
		 delete m_SellDocHent;
 }

 const CowPresenceKey& CowPresenceView::GetKey() const 
 {
	 return m_Key;
 }

bool  CowPresenceView::IsLocked() const
{
	return m_Locked;
}

bool  CowPresenceView::IsInStock() const
{
	return m_InStock;
}

const CowNo& CowPresenceView::GetCowNo() const 
{
	return m_CowNo;
}

const std::wstring& CowPresenceView::GetPassNo() const 
{
	return m_PassNo;
}

const HentView* CowPresenceView::GetFirstOwner() const 
{
	return m_FirstOwner;
}

const DateTime& CowPresenceView::GetBirthDate() const 
{
	return m_BirthDate;
}

const Decimal& CowPresenceView::GetWeight() const 
{
	return m_Weight;
}

const CowSex& CowPresenceView::GetSex() const 
{
	return m_Sex;
}

const std::wstring& CowPresenceView::GetOwnStockCd() const 
{
	return m_OwnStockCd;
}

const std::wstring& CowPresenceView::GetOwnClassCd() const 
{
	return m_OwnClassCd;
}

const std::wstring& CowPresenceView::GetExtras() const 
{
	return m_Extras;
}

const HentView* CowPresenceView::GetBuyFromHent() const 
{
	return m_BuyFromHent;
}

const std::wstring& CowPresenceView::GetBuyStockCd() const 
{
	return m_BuyStockCd;
}

const NullDecimal& CowPresenceView::GetBuyPrice() const 
{
	return m_BuyPrice;
}

const NullDecimal& CowPresenceView::GetBuyWeight() const 
{
	return m_BuyWeight;
}

const std::wstring& CowPresenceView::GetHerdInDocNo() const 
{
	return m_HerdInDocNo;
}

const DateTime& CowPresenceView::GetHerdInDate() const 
{
	return m_HerdInDate;
}

const NullInt& CowPresenceView::GetHerdInGrp() const 
{
	return m_HerdInGrp;
}

const std::wstring& CowPresenceView::GetHerdInClassCd() const 
{
	return m_HerdInClassCd;
}

const NullDecimal& CowPresenceView::GetHerdInWeight() const 
{
	return m_HerdInWeight;
}

const HentView* CowPresenceView::GetSellToHent() const 
{
	return m_SellToHent;
}

const std::wstring& CowPresenceView::GetSellStockCd() const 
{
	return m_SellStockCd;
}

const NullDecimal& CowPresenceView::GetSellPrice() const 
{
	return m_SellPrice;
}

const NullDecimal& CowPresenceView::GetSellWeight() const 
{
	return m_SellWeight;
}

const std::wstring& CowPresenceView::GetHerdOutDocNo() const 
{
	return m_HerdOutDocNo;
}

const DateTime& CowPresenceView::GetHerdOutDate() const 
{
	return m_HerdOutDate;
}

const NullInt& CowPresenceView::GetHerdOutGrp() const 
{
	return m_HerdOutGrp;
}

const std::wstring& CowPresenceView::GetHerdOutClassCd() const 
{
	return m_HerdOutClassCd;
}

const NullDecimal& CowPresenceView::GetHerdOutWeight() const 
{
	return m_HerdOutWeight;
}

const std::wstring& CowPresenceView::GetBuyInvoiceNo() const 
{
	return m_BuyInvoiceNo;
}

const std::wstring& CowPresenceView::GetTermBuyClassCd() const 
{
	return m_TermBuyClassCd;
}

const std::wstring& CowPresenceView::GetTermSellClassCd() const 
{
	return m_TermSellClassCd;
}

const NullDecimal& CowPresenceView::GetTermBuyWeight() const 
{
	return m_TermBuyWeight;
}

const NullDecimal& CowPresenceView::GetTermSellWeight() const 
{
	return m_TermSellWeight;
}

const NullDecimal& CowPresenceView::GetTermBuyPrice() const 
{
	return m_TermBuyPrice;
}

const NullDecimal& CowPresenceView::GetTermSellPrice() const 
{
	return m_TermSellPrice;
}


const CowNo& CowPresenceView::GetMotherNo() const
{
	return m_motherNo;
}

Motive CowPresenceView::GetHerdInMotive() const
{
	return m_herdInMotive;
}

Motive CowPresenceView::GetHerdOutMotive() const
{
	return m_herdOutMotive;
}


void CowPresenceView::SetLocked(bool bLocked)
{
	m_Locked = bLocked;
}

void CowPresenceView::SetInStock(bool bInStock)
{
	m_InStock = bInStock;
}

void CowPresenceView::SetCowNo(const CowNo& cowNo)
{
	 m_CowNo = cowNo;
}

void CowPresenceView::SetPassNo(const std::wstring& sPassNo)
{
	 m_PassNo = sPassNo;
}

void CowPresenceView::SetFirstOwner(const HentView* pHent)
{
	delete m_FirstOwner;
	m_FirstOwner = NULL;

	if(pHent)
	{
		m_FirstOwner = new HentView(*pHent);
	}	
}

void CowPresenceView::SetBirthDate(const DateTime& birthDate)
{
	 m_BirthDate = birthDate;
}

void CowPresenceView::SetWeight(const Decimal& weight)
{
	 m_Weight = weight;
}

void CowPresenceView::SetSex(const CowSex& sex)
{
	 m_Sex = sex;
}

void CowPresenceView::SetOwnStockCd(const std::wstring& sStockCd)
{
	 m_OwnStockCd = sStockCd;
}

void CowPresenceView::SetOwnClassCd(const std::wstring& sClassCd)
{
	 m_OwnClassCd = sClassCd;
}

void CowPresenceView::SetExtras(const std::wstring& sExtras)
{
	 m_Extras = sExtras;
}

void CowPresenceView::SetBuyFromHent(const HentView* pHent)
{
	delete m_BuyFromHent;
	m_BuyFromHent = NULL;

	if(pHent)
	{
		m_BuyFromHent = new HentView(*pHent);
	}	
}


void CowPresenceView::SetBuyClassCd(const std::wstring& sBuyClassCd)
{
	 m_BuyStockCd = sBuyClassCd;
}

void CowPresenceView::SetBuyPrice(const NullDecimal& buyPrice)
{
	 m_BuyPrice = buyPrice;
}

void CowPresenceView::SetBuyWeight(const NullDecimal& buyWeight)
{
	 m_BuyWeight = buyWeight;
}


void CowPresenceView::SetHerdInDocNo(const std::wstring& herdInDocNo)
{
	 m_HerdInDocNo = herdInDocNo;
}

void CowPresenceView::SetHerdInDate(const DateTime& herdInDate)
{
	 m_HerdInDate = herdInDate;
}

void CowPresenceView::SetHerdInGrp(const NullInt& herdInGrp)
{
	 m_HerdInGrp = herdInGrp;
}

void CowPresenceView::SetHerdInClassCd(const std::wstring& sHerdInClassCd)
{
	 m_HerdInClassCd = sHerdInClassCd;
}

void CowPresenceView::SetHerdInWeight(const NullDecimal& herdInWeight)
{
	 m_HerdInWeight = herdInWeight;
}

void CowPresenceView::SetSellToHent(const HentView* pHent)
{
	delete m_SellToHent;
	m_SellToHent = NULL;

	if(pHent)
	{
		m_SellToHent = new HentView(*pHent);
	}	
}

void CowPresenceView::SetSellClassCd(const std::wstring& sSellClassCd)
{
	 m_SellStockCd = sSellClassCd;
}

void CowPresenceView::SetSellPrice(const NullDecimal& sellPrice)
{
	 m_SellPrice = sellPrice;
}

void CowPresenceView::SetSellWeight(const NullDecimal& sellWeight)
{
	 m_SellWeight = sellWeight;
}

void CowPresenceView::SetHerdOutDocNo(const std::wstring& herdOutDocNo)
{
	 m_HerdOutDocNo = herdOutDocNo;
}

void CowPresenceView::SetHerdOutDate(const DateTime& herdOutDate)
{
	 m_HerdOutDate = herdOutDate;
}

void CowPresenceView::SetHerdOutGrp(const NullInt& herdOutGrp)
{
	 m_HerdOutGrp = herdOutGrp;
}

void CowPresenceView::SetHerdOutClassCd(const std::wstring& sHerdOutClassCd)
{
	 m_HerdOutClassCd = sHerdOutClassCd;
}

void CowPresenceView::SetHerdOutWeight(const NullDecimal& herdOutWeight)
{
	 m_HerdOutWeight = herdOutWeight;
}

void CowPresenceView::SetBuyInvoiceNo(const std::wstring& sInvoiceNo)
{
	 m_BuyInvoiceNo = sInvoiceNo;
}

void CowPresenceView::SetTermBuyClassCd(const std::wstring& sTermBuyClassCd)
{
	 m_TermBuyClassCd = sTermBuyClassCd;
}

void CowPresenceView::SetTermSellClassCd(const std::wstring& TermSellClassCd)
{
	 m_TermSellClassCd = TermSellClassCd;
}

void CowPresenceView::SetTermBuyWeight(const NullDecimal& termBuyWeight)
{
	 m_TermBuyWeight = termBuyWeight;
}

void CowPresenceView::SetTermSellWeight(const NullDecimal& termSellWeight)
{
	 m_TermSellWeight = termSellWeight;
}

void CowPresenceView::SetTermBuyPrice(const NullDecimal& termBuyPrice)
{
	 m_TermBuyPrice = termBuyPrice;
}

void CowPresenceView::SetTermSellPrice(const NullDecimal& termSellPrice)
{
	 m_TermSellPrice = termSellPrice;
}

const std::wstring& CowPresenceView::GetSellInvoiceNo() const
{
	return m_SellInvoiceNo;
}

void CowPresenceView::SetSellInvoiceNo(const std::wstring& sInvoiceNo)
{
	m_SellInvoiceNo = sInvoiceNo;
}

const std::wstring& CowPresenceView::GetHerdOutPlateNo() const
{
	return m_HerdOutPlateNo;
}

const std::wstring& CowPresenceView::GetHerdInPlateNo() const
{
	return m_HerdInPlateNo;
}

const std::wstring& CowPresenceView::GetHealthCertNo() const
{
	return m_healthCertNo;
}

void CowPresenceView::SetHerdInPlateNo(const std::wstring& sHerdInPlateNo)
{
	m_HerdInPlateNo = sHerdInPlateNo;
}

void CowPresenceView::SetHerdOutPlateNo(const std::wstring& sHerdOutPlateNo)
{
	m_HerdOutPlateNo = sHerdOutPlateNo;
}

void CowPresenceView::SetMotherNo(const CowNo& motherNo)
{
	m_motherNo = motherNo;
}

void CowPresenceView::SetOwnStockId(uint32_t stockId)
{
	m_stockId = stockId;
}

void CowPresenceView::SetOwnClassId(uint32_t classId)
{
	m_classId = classId;
}

uint32_t CowPresenceView::GetOwnStockId() const
{
	return m_stockId;
}

uint32_t CowPresenceView::GetOwnClassId() const
{
	return m_classId;
}

void CowPresenceView::SetHealthCertNo(const std::wstring& healthCertNo)
{
	m_healthCertNo = healthCertNo;
}


void CowPresenceView::SetBuyDocHent(const HentView* pHent)
{
	delete m_BuyDocHent;
	m_BuyDocHent = NULL;

	if (pHent)
	{
		m_BuyDocHent = new HentView(*pHent);
	}
}

const HentView* CowPresenceView::GetBuyDocHent() const
{
	return m_BuyDocHent;
}

void CowPresenceView::SetSellDocHent(const HentView* pHent)
{
	delete m_SellDocHent;
	m_SellDocHent = NULL;

	if (pHent)
	{
		m_SellDocHent = new HentView(*pHent);
	}
}

const HentView* CowPresenceView::GetSellDocHent() const
{
	return m_SellDocHent;
}


void CowPresenceView::SetHerdInMotive(Motive motive)
{
	m_herdInMotive = motive;
}

void CowPresenceView::SetHerdOutMotive(Motive motive)
{
	m_herdOutMotive = motive;
}