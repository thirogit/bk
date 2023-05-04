#ifndef __PURCHASECOW_H__
#define __PURCHASECOW_H__

#include <data/types/CowSex.h>
#include <types/Decimal.h>
#include <types/DateTime.h>

#include "IPurchaseCow.h"

class SerializablePurchaseCow;

class PurchaseCow : public IPurchaseCow
{
public:	
	PurchaseCow(const std::wstring& id);
	
	void CopyFrom(const IPurchaseCow* src);
	void CopyFrom(const PurchaseCow& src);

	const std::wstring& GetId() const;
	const CowNo& GetCowNo() const;
	const CowSex& GetSex() const;	
	const std::wstring& GetStockCd() const;
	const std::wstring& GetClassCd() const;
	const Decimal& GetWeight() const;	
	const Decimal& GetPrice() const;	
	const Latitude& GetLatitude() const;
	const Longitude& GetLongitude() const;
	const std::wstring& GetPassportNo() const;	
	const HentNo& GetFirstOwner() const;
	const DateTime& GetPassportIssueDt() const;
	const std::wstring& GetHealthCertNo() const;
	const CowNo& GetMotherNo() const;
	const std::wstring& GetBirthPlace() const;
	const DateTime& GetBirthDt() const;
	void SetCowNo(const CowNo& cowNo);
	void SetSex(const CowSex& sex);	
	void SetStockCd(const std::wstring& sStockCode);
	void SetClassCd(const std::wstring& sClassCd);
	void SetWeight(const Decimal& weight);	
	void SetPrice(const Decimal& price);
	void SetLatitude(const Latitude& latitude);
	void SetLongitude(const Longitude& longitude);
	void SetPassportNo(const std::wstring& sPassportNo);	
	void SetFirstOwner(const HentNo& firstOwnerNo);
	void SetPassportIssueDt(const DateTime& passportIssueDt);
	void SetHealthCertNo(const std::wstring& sHealthCertNo);
	void SetMotherNo(const CowNo& motherNo);
	void SetBirthPlace(const std::wstring& sBirthPlace);
	void SetBirthDt(const DateTime& birthDt);


private:
	std::wstring m_id;
	CowNo m_CowNo;
	CowSex m_Sex;	
	std::wstring m_StockCode;
	std::wstring m_ClassCode;
	Decimal m_Weight;	
	Decimal m_Price;	
	Latitude m_Latitude;
	Longitude m_Longitude;
	std::wstring m_PassportNo;	
	HentNo m_FirstOwnerNo;
	DateTime m_PassportIssueDt;
	std::wstring m_HealthCertNo;
	CowNo m_MotherNo;
	std::wstring m_BirthPlace;
	DateTime m_BirthDt;

	friend class SerializablePurchaseCow;
};
	

#endif
