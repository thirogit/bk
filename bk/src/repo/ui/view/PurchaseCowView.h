#ifndef __PURCHASECOWVIEW_H__
#define __PURCHASECOWVIEW_H__

#include <stdint.h>
#include <string>
#include <data\types\CowNo.h>
#include <data\types\HentNo.h>
#include <data\types\CowSex.h>
#include <types\Decimal.h>
#include <types\DateTime.h>
#include <types\NullDecimal.h>

namespace PurchaseCowViewMember
{
	enum 
	{
		CowNo,
		PassportNo,
		FirstOwnerNo,
		BirthDate,
		Weight,
		Sex,
		StockCd,
		ClassCd,
		Price,
		Latitude,
	    Longitude,
	    PassportIssueDt,
	    HealthCertNo,
	    MotherNo,
	    BirthPlace
	};
};
	
	
class PurchaseCowView
{
public:
	PurchaseCowView(const std::wstring& id);

	const std::wstring& GetId() const;

	const Decimal& GetPrice() const;	
	void SetPrice(const Decimal& price);
	
	const Decimal& GetWeight() const;	
	void SetWeight(const Decimal& weight);

	const std::wstring& GetStockCd() const;	
	void SetStockCd(const std::wstring& stockCd);

	const std::wstring& GetClassCd() const;		
	void SetClassCd(const std::wstring& classCd);

	const CowNo& GetCowNo() const;	
	void SetCowNo(const CowNo& cowNo);

	const CowNo& GetMotherNo() const;
	void SetMotherNo(const CowNo& motherNo);

	const DateTime& GetBirthDate() const;
	void SetBirthDate(const DateTime& birthDt);

	const std::wstring& GetPassNo() const;
	void SetPassNo(const std::wstring& passNo);

	const std::wstring& 	GetHealthCertNo() const;
	void SetHealthCertNo(const std::wstring& sHealthCertNo);

	const DateTime&	GetPassDate() const;	
	void SetPassDate(const DateTime& passDt);

	const std::wstring& GetBirthPlace() const;
	void SetBirthPlace(const std::wstring& birthPlace);
	
	const HentNo& GetFirstOwnerNo() const;
	void SetFirstOwnerNo(const HentNo& fstOwnerNo);
	
	const CowSex& GetSex() const;
	void SetSex(const CowSex& cowsex);	
	
	
	
private:
	std::wstring 	m_Id;
	Decimal			m_weight;
	Decimal			m_price;
	std::wstring	m_stockCd;	
	std::wstring	m_classCd;	
	CowNo			m_cowno;
	DateTime		m_birthdate;
	std::wstring	m_birthplace;
	CowSex			m_sex;
	CowNo			m_motherNo;	
	std::wstring	m_passno;
	DateTime		m_passdate;
	HentNo			m_fstownerNo;	
	std::wstring	m_healthcertno;
	
};

#endif