#pragma once

#include <types\NullDecimal.h>
#include <types\DateTime.h>
#include <types\NullInt.h>
#include <string>
#include <data\types\CowNo.h>
#include <data\types\CowSex.h>
#include <logic\herdregistry\CowPresenceKey.h>
#include "HentView.h"
#include <data\Motive.h>

struct CowPresenceViewMember
{
	enum
	{
		State,
		CowNo,
		PassNo,	
		FirstOwner,
		BirthDate,
		Weight,
		Sex,
		StockCd,
		ClassCd,
		Extras,

		
		BuyFromHent,
		BuyStock,
		BuyPrice,
		BuyWeight,

		HerdInDocNo,
		HerdInDate,
		HerdInGrp,
		HerdInClass,
		HerdInWeight,
		HerdInMotive,
	 	 
		SellToHent,
		SellStock,
		SellPrice,
		SellWeight,

		HerdOutDocNo,	 
		HerdOutDate,
		HerdOutGrp,
		HerdOutClass,	
	  
		BuyInvoiceNo,
		SellInvoiceNo,
	 
		TermBuyClass,
		TermSellClass,
		TermBuyWeight,
		TermSellWeight,
		TermBuyPrice,
		TermSellPrice,	 

		StockId,
		ClassId,

		BuyDocHent,
		HerdOutWeight,
		HerdOutMotive,
	};
};

class CowPresenceView 
{
public:
	CowPresenceView(const CowPresenceKey& key);

	CowPresenceView(const CowPresenceView& src);

	~CowPresenceView();
	const CowPresenceKey& GetKey() const ;
	bool  IsLocked() const;	
	bool  IsInStock() const;	
	const CowNo& GetCowNo() const;
	const std::wstring& GetPassNo() const;
	const HentView* GetFirstOwner() const;
	const DateTime& GetBirthDate() const;
	const Decimal& GetWeight() const;
	const CowSex& GetSex() const;
	const std::wstring& GetOwnStockCd() const;
	const std::wstring& GetOwnClassCd() const;
	const std::wstring& GetExtras() const;
	const HentView* GetBuyFromHent() const;
	const std::wstring& GetBuyStockCd() const;
	const NullDecimal& GetBuyPrice() const;
	const NullDecimal& GetBuyWeight() const;
	const std::wstring& GetHerdInDocNo() const;
	const DateTime& GetHerdInDate() const;
	const NullInt& GetHerdInGrp() const;
	const std::wstring& GetHerdInClassCd() const;
	const NullDecimal& GetHerdInWeight() const;
	const std::wstring& GetHerdInPlateNo() const;
	const HentView* GetSellToHent() const;
	const std::wstring& GetSellStockCd() const;
	const NullDecimal& GetSellPrice() const;
	const NullDecimal& GetSellWeight() const;
	const std::wstring& GetHerdOutDocNo() const;
	const DateTime& GetHerdOutDate() const;
	const NullInt& GetHerdOutGrp() const;
	const std::wstring& GetHerdOutClassCd() const;
	const NullDecimal& GetHerdOutWeight() const;
	const std::wstring& GetHerdOutPlateNo() const;
	const std::wstring& GetBuyInvoiceNo() const;
	const std::wstring& GetSellInvoiceNo() const;
	const std::wstring& GetTermBuyClassCd() const;
	const std::wstring& GetTermSellClassCd() const;
	const NullDecimal& GetTermBuyWeight() const;
	const NullDecimal& GetTermSellWeight() const;
	const NullDecimal& GetTermBuyPrice() const;
	const NullDecimal& GetTermSellPrice() const;
	const std::wstring& GetHealthCertNo() const;
	const CowNo& GetMotherNo() const;
	Motive GetHerdInMotive() const;
	Motive GetHerdOutMotive() const;
	uint32_t GetOwnStockId() const;
	uint32_t GetOwnClassId() const;
	const HentView* GetBuyDocHent() const;
	const HentView* GetSellDocHent() const;
	
	void SetLocked(bool bLocked);	
	void SetInStock(bool bInStock);	
	void SetCowNo(const CowNo& CowNo);
	void SetPassNo(const std::wstring& PassNo);
	void SetFirstOwner(const HentView* pHent);
	void SetBirthDate(const DateTime& brithDate);
	void SetWeight(const Decimal& weight);
	void SetSex(const CowSex& sex);
	void SetOwnStockCd(const std::wstring& sStockCd);
	void SetOwnClassCd(const std::wstring& sClassCd);
	void SetExtras(const std::wstring& sExtras);

	void SetBuyFromHent(const HentView* pHent);
	void SetBuyClassCd(const std::wstring& sBuyClassCd);
	void SetBuyPrice(const NullDecimal& buyPrice);
	void SetBuyWeight(const NullDecimal& buyWeight);

	void SetHerdInDocNo(const std::wstring& sHerdInDocNo);
	void SetHerdInDate(const DateTime& herdInDate);
	void SetHerdInGrp(const NullInt& hernInGrp);
	void SetHerdInClassCd(const std::wstring& sHerdInClassCd);
	void SetHerdInWeight(const NullDecimal& herdInWeight);
	void SetHerdInPlateNo(const std::wstring& sHerdInPlateNo);

	void SetSellToHent(const HentView* pHent);
	void SetSellClassCd(const std::wstring& sSellClassCd);
	void SetSellPrice(const NullDecimal& sellPrice);
	void SetSellWeight(const NullDecimal& sellWeight);

	void SetHerdOutDocNo(const std::wstring& herdOutDocNo);
	void SetHerdOutDate(const DateTime& herdOutDate);
	void SetHerdOutGrp(const NullInt& herdOutGrp);
	void SetHerdOutClassCd(const std::wstring& sHerdOutClassCd);
	void SetHerdOutWeight(const NullDecimal& herdOutWeight);
	void SetHerdOutPlateNo(const std::wstring& sHerdOutPlateNo);

	void SetBuyInvoiceNo(const std::wstring& sInvoiceNo);
	void SetSellInvoiceNo(const std::wstring& sInvoiceNo);
	void SetTermBuyClassCd(const std::wstring& sTermBuyStockCd);
	void SetTermSellClassCd(const std::wstring& sTermSellStockCd);
	void SetTermBuyWeight(const NullDecimal& termBuyWeight);
	void SetTermSellWeight(const NullDecimal& termSellWeight);
	void SetTermBuyPrice(const NullDecimal& termBuyPrice);
	void SetTermSellPrice(const NullDecimal& termSellPrice);
	void SetHealthCertNo(const std::wstring& healthCertNo);
	void SetMotherNo(const CowNo& motherNo);
	void SetHerdInMotive(Motive motive);
	void SetHerdOutMotive(Motive motive);

	void SetOwnStockId(uint32_t stockId);
	void SetOwnClassId(uint32_t classId);
	void SetBuyDocHent(const HentView* pHent);
	void SetSellDocHent(const HentView* pHent);
	
	std::wstring toPlaceStr;
	std::wstring carrierStr;
	
private:
	CowPresenceKey	m_Key;
	bool			m_Locked;
	
	HentView* 		m_BuyDocHent;
	HentView* 		m_SellDocHent;

	bool			m_InStock;	
	CowNo			m_CowNo;
	std::wstring		m_PassNo;
	HentView* 		m_FirstOwner;
	DateTime		m_BirthDate;
	Decimal			m_Weight;
	CowSex			m_Sex;
	std::wstring		m_OwnStockCd;
	std::wstring		m_OwnClassCd;
	std::wstring		m_Extras;
	HentView* 		m_BuyFromHent;
	
	std::wstring		m_BuyStockCd;
	NullDecimal		m_BuyPrice;
	NullDecimal		m_BuyWeight;
	std::wstring		m_HerdInDocNo;
	DateTime		m_HerdInDate;
	NullInt			m_HerdInGrp;
	std::wstring		m_HerdInClassCd;
	NullDecimal		m_HerdInWeight;
	std::wstring		m_HerdInPlateNo;
	HentView* 		m_SellToHent;
	std::wstring		m_SellStockCd;
	NullDecimal		m_SellPrice;
	NullDecimal		m_SellWeight;
	std::wstring		m_HerdOutDocNo;
	DateTime		m_HerdOutDate;
	NullInt			m_HerdOutGrp;
	std::wstring		m_HerdOutClassCd;
	NullDecimal		m_HerdOutWeight;
	std::wstring		m_HerdOutPlateNo;
	std::wstring		m_BuyInvoiceNo;
	std::wstring		m_SellInvoiceNo;
	std::wstring		m_TermBuyClassCd;
	std::wstring		m_TermSellClassCd;
	NullDecimal		m_TermBuyWeight;
	NullDecimal		m_TermSellWeight;
	NullDecimal		m_TermBuyPrice;
	NullDecimal		m_TermSellPrice;
	std::wstring		m_healthCertNo;
	CowNo			m_motherNo;
	Motive			m_herdInMotive;
	Motive			m_herdOutMotive;
	uint32_t		m_stockId;
	uint32_t		m_classId;
};
