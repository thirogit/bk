#ifndef __COW_H__
#define __COW_H__

#include "../ICow.h"
#include "CowDetails.h"

class SerializableCow;

class Cow : public ICow
{
public:
	
	Cow(int Id = NULL_ID);
	~Cow();

	virtual void CopyFrom(const ICow* pSrc);
	void SetFrom(const CowDetails& details);
	const CowDetails& GetDetails() const;
	
	uint32_t	GetId() const;
	
	virtual const Decimal&		GetWeight() const;	
	void						SetWeight(const Decimal& weight);

	virtual uint32_t			GetStockId() const;	
	void						SetStockId(uint32_t stockId);

	virtual uint32_t			GetClassId() const;		
	void						SetClassId(uint32_t classId);

	virtual const CowNo&		GetCowNo() const;	
	void						SetCowNo(const CowNo& cowNo);

	virtual const CowNo& 		GetMotherNo() const;
	void						SetMotherNo(const CowNo& motherNo);

	virtual const DateTime&		GetBirthDate() const;
	void						SetBirthDate(const DateTime& birthDt);

	virtual const std::wstring&	GetPassNo() const;
	void						SetPassNo(const std::wstring& passNo);

	virtual const std::wstring& GetHealthCertNo() const;
	void						SetHealthCertNo(const std::wstring& sHealthCertNo);

	virtual const DateTime&		GetPassDate() const;	
	void						SetPassDate(const DateTime& passDt);

	virtual const std::wstring&	GetBirthPlace() const;
	void						SetBirthPlace(const std::wstring& birthPlace);

	virtual const std::wstring& GetExtras() const;
	void						SetExtras(const std::wstring& sExtras);		

	virtual uint32_t			GetBuyInvoiceId() const;
			void				SetBuyInvoiceId(uint32_t invoiceId);

	virtual uint32_t			GetSellInvoiceId() const;
			void				SetSellInvoiceId(uint32_t invoiceId);
		
	virtual uint32_t 			GetFirstOwnerId() const;
			void				SetFirstOwnerId(uint32_t hentId);
	
	virtual const CowSex&		GetSex() const;
			void				SetSex(const CowSex& cowsex);	
	
	virtual const NullDecimal&	GetTermBuyPrice() const;
			void				SetTermBuyPrice(const NullDecimal& dTermBuyPrice) ;

	virtual const NullDecimal&	GetTermSellPrice() const;
			void				SetTermSellPrice(const NullDecimal& dTermSellPrice) ;

	virtual const NullDecimal&	GetTermBuyWeight() const;
			void				SetTermBuyWeight(const NullDecimal& buyWeight) ;

	virtual const NullDecimal&	GetTermSellWeight() const;
			void				SetTermSellWeight(const NullDecimal& sellWeight);

	virtual uint32_t			GetTermBuyClassId() const;
			void				SetTermBuyClassId(uint32_t classId) ;

	virtual uint32_t			GetTermSellClassId() const;
			void				SetTermSellClassId(uint32_t classId) ;

	virtual const std::wstring& 	GetPassImgLabel() const;
			void				SetPassImgLabel(const std::wstring& sPassImgLabel);
			
protected:
	uint32_t		m_cowId;	
	std::wstring	m_passimglabel;		
	
	uint32_t		m_sellInvoice;
	uint32_t		m_buyInvoice;
	CowDetails		m_details;	
	

	friend class SerializableCow;
	
};



#endif