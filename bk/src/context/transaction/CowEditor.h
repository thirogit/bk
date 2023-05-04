#ifndef __COWEDITOR_H__
#define __COWEDITOR_H__

#include <string>
#include <data\impl\CowDetails.h>

class CowEditor
{
public:
	
	virtual bool IsEditing() const = 0;
	virtual void Save() = 0;
	
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
			
protected:
	CowDetails m_Details;
};

#endif