#ifndef __COWVIEW_H__
#define __COWVIEW_H__

#include <stdint.h>
#include <string>
#include <data\types\CowNo.h>
#include <data\types\CowSex.h>
#include <types\Decimal.h>
#include <types\DateTime.h>
#include <types\NullDecimal.h>

namespace CowViewMembers
{
	enum CowViewMember
	{
		CowView_CowNo,
		CowView_PassNo,
		CowView_FirstOwner,
		CowView_BirthDate,
		CowView_Weight,
		CowView_Sex,
		CowView_Stock,
		CowView_Class,
		CowView_TermBuyClass,		
		CowView_TermBuyWeight,		
		CowView_TermBuyPrice,	
		CowView_TermSellClass,		
		CowView_TermSellWeight,	
		CowView_TermSellPrice,
	};
};

class CowView
{
public:
	CowView(uint32_t id);

	uint32_t GetId() const;	

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

	const std::wstring& 	GetExtras() const;
	void SetExtras(const std::wstring& sExtras);	
		
	const std::wstring& GetFirstOwnerAlias() const;
	void SetFirstOwnerAlias(const std::wstring& alias);
	
	const CowSex& GetSex() const;
	void SetSex(const CowSex& cowsex);	
	
	const NullDecimal&	GetTermBuyPrice() const;
	void SetTermBuyPrice(const NullDecimal& dTermBuyPrice) ;

	const NullDecimal&	GetTermSellPrice() const;
	void SetTermSellPrice(const NullDecimal& dTermSellPrice) ;

	const NullDecimal&	GetTermBuyWeight() const;
	void SetTermBuyWeight(const NullDecimal& buyWeight) ;

	const NullDecimal&	GetTermSellWeight() const;
	void SetTermSellWeight(const NullDecimal& sellWeight);

	const std::wstring& GetTermBuyClassCd() const;
	void SetTermBuyClassCd(const std::wstring& classCd) ;

	const std::wstring& GetTermSellClassCd() const;
	void SetTermSellClassCd(const std::wstring& classCd) ;
	
private:
	uint32_t 		m_Id;
	Decimal			m_weight;
	std::wstring		m_stockCd;	
	std::wstring		m_classCd;	
	CowNo			m_cowno;
	DateTime		m_birthdate;
	std::wstring		m_birthplace;
	CowSex			m_sex;
	CowNo			m_motherNo;	
	std::wstring		m_passno;
	DateTime		m_passdate;
	std::wstring		m_fstownerAlias;
	std::wstring		m_extras;		
	std::wstring		m_healthcertno;
	std::wstring		m_termbuyclassCd;
	NullDecimal		m_termbuyweight;
	std::wstring		m_termsellclassCd;
	NullDecimal		m_termsellweight;
	NullDecimal		m_termbuyprice;
	NullDecimal		m_termsellprice;
};

#endif