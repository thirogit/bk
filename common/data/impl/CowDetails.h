#ifndef __COWDETAILS_H__
#define __COWDETAILS_H__

#include "../ICow.h"

class SerializableCowDetails;

class CowDetails
{
public:
	CowDetails();
	CowDetails(const CowDetails& src);

	void CopyFrom(const CowDetails& src);
	void CopyFrom(const ICow* pSrc);
	
	const Decimal& GetWeight() const;	
	void  SetWeight(const Decimal& weight);

	uint32_t GetStockId() const;	
	void SetStockId(uint32_t stockId);

	uint32_t GetClassId() const;		
	void SetClassId(uint32_t classId);

	const CowNo& GetCowNo() const;	
	void SetCowNo(const CowNo& cowNo);

	const CowNo& GetMotherNo() const;
	void SetMotherNo(const CowNo& motherNo);

	const DateTime&	GetBirthDate() const;
	void SetBirthDate(const DateTime& birthDt);

	const std::wstring& GetPassNo() const;
	void SetPassNo(const std::wstring& passNo);

	const std::wstring& GetHealthCertNo() const;
	void SetHealthCertNo(const std::wstring& sHealthCertNo);

	const DateTime&	GetPassDate() const;	
	void SetPassDate(const DateTime& passDt);

	const std::wstring& GetBirthPlace() const;
	void SetBirthPlace(const std::wstring& birthPlace);

	const std::wstring& GetExtras() const;
	void SetExtras(const std::wstring& sExtras);
		
	uint32_t GetFirstOwnerId() const;
	void SetFirstOwnerId(uint32_t hentId);

	const CowSex& GetSex() const;
	void SetSex(const CowSex& cowsex);	

	const NullDecimal& GetTermBuyPrice() const;
	void SetTermBuyPrice(const NullDecimal& dTermBuyPrice) ;

	const NullDecimal&	GetTermSellPrice() const;
	void SetTermSellPrice(const NullDecimal& dTermSellPrice) ;

	const NullDecimal&	GetTermBuyWeight() const;
	void SetTermBuyWeight(const NullDecimal& buyWeight) ;

	const NullDecimal&	GetTermSellWeight() const;
	void SetTermSellWeight(const NullDecimal& sellWeight);

	uint32_t GetTermBuyClassId() const;
	void SetTermBuyClassId(uint32_t classId) ;

	uint32_t GetTermSellClassId() const;
	void SetTermSellClassId(uint32_t classId) ;

	
		
protected:
	Decimal			m_weight;
	uint32_t		m_stockId;	
	uint32_t		m_classId;	
	CowNo			m_cowno;
	DateTime		m_birthdate;
	std::wstring	m_birthplace;
	CowSex			m_sex;
	CowNo			m_motherNo;	
	std::wstring	m_passno;
	DateTime		m_passdate;
	uint32_t		m_fstownerId;
	std::wstring	m_extras;		
	std::wstring	m_healthcertno;
	uint32_t		m_termbuyclassId;
	NullDecimal		m_termbuyweight;
	uint32_t		m_termsellclassId;
	NullDecimal		m_termsellweight;
	NullDecimal		m_termbuyprice;
	NullDecimal		m_termsellprice;	

	friend class SerializableCowDetails;	
};



#endif