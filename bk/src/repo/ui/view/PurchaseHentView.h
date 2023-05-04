#ifndef __PURCHASEHENTVIEW_H__
#define __PURCHASEHENTVIEW_H__

#include <string>
#include <stdint.h>
#include <types\DateTime.h>
#include <data/types/HentNo.h>
#include <data/HentType.h>
#include <data\types\AccountNo.h>
#include <types\Latitude.h>
#include <types\Longitude.h>

struct PurchaseHentViewMember
{
	enum
	{		
		Name,
		HentNo,
		Alias,
		Street,
		POBox,
		City,
		Zip,
		PlateNo,
		Type,
		PhoneNo,
		FiscalNo,
		WetNo,
		PersonalNo,
		StatNo,
		PersonalIdNo,
		PersonalIdIssueDt,
		PersonalIdIssuePost,
		AccountNo,
		BankName,
		WetLicenceNo,
		CellPhoneNo,
		EmailAddress,
		Latitude,
		Longitude	
	};
};


class PurchaseHentView 
{
public:
	PurchaseHentView(const std::wstring& hentId);
	const std::wstring& GetId() const;

	void CopyFrom(const PurchaseHentView& src);
			
	const std::wstring& GetName() const;
	void SetName(const std::wstring& sName);

	const std::wstring& GetAlias() const;
	void SetAlias(const std::wstring& sAlias);

	const std::wstring& GetStreet() const;
	void SetStreet(const std::wstring& sStreet);
		
	const std::wstring& GetPOBox() const;
	void SetPOBox(const std::wstring& sPOBox);
	
	const std::wstring& GetCity() const;
	void SetCity(const std::wstring& sCity);
	
	const std::wstring& GetZip() const;
	void SetZip(const std::wstring& sZip);
	
	virtual const std::wstring& GetPhoneNo() const;
	void SetPhoneNo(const std::wstring& sPhoneNo);
	
	virtual const std::wstring& GetFiscalNo() const;
	void SetFiscalNo(const std::wstring& sFiscalNo);
	
	virtual const HentNo& GetHentNo() const;
	void SetHentNo(const HentNo& hentNo);
	
	virtual const std::wstring& GetWetNo() const;
	void SetWetNo(const std::wstring& sWetNo);	
	
	virtual const std::wstring& GetPlateNo() const;	
	void SetPlateNo(const std::wstring& sPlate);
	
	virtual HentType GetHentType() const;
	void SetHentType(HentType hentType);
		
	virtual const std::wstring& GetPersonalNo() const;
	void SetPersonalNo(const std::wstring& sPersonalNo);
		
	virtual const std::wstring& GetStatsNo() const;
	void SetStatsNo(const std::wstring& sStatsNo);
		
	virtual const std::wstring& GetIdNo() const;
	void SetIdNo(const std::wstring& sIdNo);
		
	virtual const DateTime& GetIssueDate() const;
	void SetIssueDate(const DateTime& IdIssueDate);
		
	virtual const std::wstring& GetIssuePost() const;
	void SetIssuePost(const std::wstring& sIssuePost);

	virtual const AccountNo& GetAccountNo() const;
	void SetAccountNo(const AccountNo& accountNo);
	
	virtual const std::wstring& GetBankName() const;
	void SetBankName(const std::wstring& sBankName);

	virtual const std::wstring& GetWetLicenceNo() const;
	void SetWetLicenceNo(const std::wstring& sWetLicenceNo);
	
	virtual const std::wstring& GetCellPhoneNo() const;
	void SetCellPhoneNo(const std::wstring& sCellPhoneNo);

	virtual const std::wstring& GetEmailAddress() const;
	void SetEmailAddress(const std::wstring& sEmailAddress);

	virtual const Latitude& GetLatitude() const;
	void SetLatitude(const Latitude& lati);

	virtual const Longitude& GetLongitude() const;
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
	std::wstring m_nip;
	HentNo m_hentno;
	std::wstring m_wetno;
	std::wstring m_plate;	
	HentType m_henttype;
	std::wstring m_pesel;
	std::wstring m_regon;
	std::wstring m_idno;
	DateTime m_issuedate;
	std::wstring m_issuepost;
	AccountNo m_accountno;
	std::wstring m_bankname;
	std::wstring m_wetlicenceno;
	std::wstring m_cellphoneno;
	std::wstring m_emailaddress;
	Latitude m_latitude;
	Longitude m_longitude;
};

#endif