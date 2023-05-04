#ifndef __COWINVOICEENTRYVIEW_H__
#define __COWINVOICEENTRYVIEW_H__

#include <string>
#include <types\Decimal.h>
#include <data\types\CowNo.h>
#include <types\DateTime.h>
#include <data\types\CowSex.h>

class CowInvoiceEntryView 
{
public:
	CowInvoiceEntryView(uint32_t cowId);	
	
	uint32_t GetId() const;	

	const Decimal& GetOwnWeight() const;	
	void SetOwnWeight(const Decimal& weight);

	const std::string& GetOwnStockCd() const;	
	void SetOwnStockCd(const std::string& stockCd);

	const std::string& GetOwnClassCd() const;		
	void SetOwnClassCd(const std::string& classCd);

	const CowNo& GetCowNo() const;	
	void SetCowNo(const CowNo& cowNo);

	const CowNo& GetMotherNo() const;
	void SetMotherNo(const CowNo& motherNo);

	const DateTime& GetBirthDate() const;
	void SetBirthDate(const DateTime& birthDt);

	const std::string& GetPassNo() const;
	void SetPassNo(const std::string& passNo);

	const std::string& 	GetHealthCertNo() const;
	void SetHealthCertNo(const std::string& sHealthCertNo);

	const DateTime&	GetPassDate() const;	
	void SetPassDate(const DateTime& passDt);

	const std::string& GetBirthPlace() const;
	void SetBirthPlace(const std::string& birthPlace);

	
	const CowSex& GetSex() const;
	void SetSex(const CowSex& cowsex);	
	
private:
	uint32_t 		m_cowId;
	Decimal			m_ownWeight;
	std::string		m_ownStockCd;	
	std::string		m_ownClassCd;	
	CowNo			m_cowNo;
	DateTime		m_birthdate;
	std::string		m_birthplace;
	CowSex			m_sex;
	CowNo			m_motherNo;	
	std::string		m_passno;
	DateTime		m_passdate;			
	std::string		m_healthcertno;
	
	Decimal			m_herdWeight;
	std::string		m_herdStockCd;	
	std::string		m_herdClassCd;	
	
	Decimal			m_invoiceWeight;
	std::string		m_invoideClassCd;	
	Decimal			m_invoicePrice;
	
	
};

#endif