#ifndef __PURCHASEHENT_H__
#define __PURCHASEHENT_H__

#include "IPurchaseHent.h"
#include "../IHent.h"

class SerializablePurchaseHent;

class PurchaseHent : public IPurchaseHent
{
public:
	PurchaseHent(const std::wstring& id);
	PurchaseHent(const PurchaseHent& src);
	
	void CopyFrom(const PurchaseHent& src);
	void CopyFrom(const IPurchaseHent* hent);
	void CopyFrom(const IHent* hent);

	const std::wstring& GetId() const;
	
	const std::wstring& GetName() const;
	void SetName(const std::wstring& hentName);
	
	const std::wstring& GetAlias() const;
	void SetAlias(const std::wstring& hentAlias);
	
	const std::wstring& GetStreet() const;
	void SetStreet(const std::wstring& hentStreet);
		
	const std::wstring& GetPOBox() const;
	void SetPOBox(const std::wstring& hentPOBox);
	
	const std::wstring& GetCity() const;
	void SetCity(const std::wstring& hentCity);
	
	const std::wstring& GetZip() const;
	void SetZip(const std::wstring& hentZip);
	
	const std::wstring& GetPhoneNo() const;
	void SetPhoneNo(const std::wstring& hentPhone);
	
	const std::wstring& GetFiscalNo() const;
	void SetFiscalNo(const std::wstring& fiscalNo);
	
	const HentNo& GetHentNo() const;
	void SetHentNo(const HentNo& hentFarmNo);
	
	const std::wstring& GetWetNo() const;
	void SetWetNo(const std::wstring& hentWetNo);
	
	const std::wstring& GetExtras() const;
	void SetExtras(const std::wstring& hentExtras);
		
	const std::wstring& GetPlateNo() const;	
	void SetPlateNo(const std::wstring& hentPlate);
	
	HentType GetHentType() const;
	void SetHentType(HentType hentType);
		
	const std::wstring& GetPersonalNo() const;
	void SetPersonalNo(const std::wstring& hentPESEL);
	
	const std::wstring& GetStatsNo() const;
	void SetStatsNo(const std::wstring& hentREGON);
	
	const std::wstring& GetPersonalIdNo() const;
	void SetPersonalIdNo(const std::wstring& hentIdNo);
	
	const DateTime& GetIssueDate() const;
	void SetIssueDate(const DateTime& hentIdIssueDate);
	
	const std::wstring& GetIssuePost() const;
	void SetIssuePost(const std::wstring& hentIssuePost);
		
	const AccountNo& GetAccountNo() const;
	void SetAccountNo(const AccountNo& hentAccountNo);
	
	const std::wstring& GetBankName() const;
	void SetBankName(const std::wstring& hentBankName);
	
	const std::wstring& GetWetLicenceNo() const;
	void SetWetLicenceNo(const std::wstring& sWetLicenceNo);
	
	const std::wstring& GetCellPhoneNo() const;
	void SetCellPhoneNo(const std::wstring& sCellPhoneNo);
	
	const std::wstring& GetEmailAddress() const;
	void SetEmailAddress(const std::wstring& sEmailAddress);
	
	const Latitude& GetLatitude() const;
	void SetLatitude(const Latitude& lati);
	
	const Longitude& GetLongitude() const;
	void SetLongitude(const Longitude& longi);

private:
	std::wstring m_id;
	std::wstring m_name;
	std::wstring m_alias;
	std::wstring m_street;
	std::wstring m_pobox;
	std::wstring m_city;
	std::wstring m_zip;
	std::wstring m_phone;
	std::wstring m_fiscalNo;
	HentNo m_hentno;
	std::wstring m_wetno;
	std::wstring m_extras;
	std::wstring m_plate;
	HentType m_henttype;
	
	std::wstring m_personalNo;
	std::wstring m_statsNo;
	std::wstring m_personalIdNo;
	DateTime m_issuedate;
	std::wstring m_issuepost;
	AccountNo m_accountno;
	std::wstring m_bankname;
	std::wstring m_wetlicenceno;
	std::wstring m_cellphoneno;
	std::wstring m_emailaddress;
	Latitude m_latitude;
	Longitude m_longitude;

	friend class SerializablePurchaseHent;
};
	

#endif