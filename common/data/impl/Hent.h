#ifndef __HENT_H__
#define __HENT_H__

#include "../IHent.h"
#include  <casajson/json.h>

class SerializableHent;

class Hent : public IHent
{
public:
	Hent(uint32_t Id = NULL_ID);
	
	~Hent();

	void CopyFrom(const IHent* src);
	
	uint32_t GetId() const;
		
	virtual const std::wstring& GetName() const;
	void SetName(const std::wstring& sName);

	virtual const std::wstring& GetAlias() const;
	void SetAlias(const std::wstring& sAlias);

	virtual const std::wstring& GetStreet() const;
	void SetStreet(const std::wstring& sStreet);
		
	virtual const std::wstring& GetPOBox() const;
	void SetPOBox(const std::wstring& sPOBox);
	
	virtual const std::wstring& GetCity() const;
	void SetCity(const std::wstring& sCity);
	
	virtual const std::wstring& GetZip() const;
	void SetZip(const std::wstring& sZip);
	
	virtual const std::wstring& GetPhoneNo() const;
	void SetPhoneNo(const std::wstring& sPhoneNo);
	
	virtual const std::wstring& GetFiscalNo() const;
	void SetFiscalNo(const std::wstring& sFiscalNo);
	bool HasFiscalNo() const;

	virtual const HentNo& GetHentNo() const;
	void SetHentNo(const HentNo& hentNo);
	
	virtual const std::wstring& GetWetNo() const;
	void SetWetNo(const std::wstring& sWetNo);	
	bool HasWetNo() const;

	virtual const std::wstring& GetExtras() const;
	void SetExtras(const std::wstring& sExtras);
		
	virtual const std::wstring& GetPlateNo() const;	
	void SetPlateNo(const std::wstring& sPlate);
	
	virtual HentType GetHentType() const;
	void SetHentType(HentType hentType);
	
	virtual bool GetSyncIt() const;
	void SetSyncIt(bool bSyncIt);
		
	virtual const std::wstring& GetPESEL() const;
	void SetPESEL(const std::wstring& sPESEL);
	bool HasPESEL() const;
	
	virtual const std::wstring& GetREGON() const;
	void SetREGON(const std::wstring& sREGON);
	bool HasREGON() const;
	
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
	bool HasWetLicenceNo() const;

	virtual const std::wstring& GetCellPhoneNo() const;
	void SetCellPhoneNo(const std::wstring& sCellPhoneNo);

	virtual const std::wstring& GetEmailAddress() const;
	void SetEmailAddress(const std::wstring& sEmailAddress);

	virtual const Latitude& GetLatitude() const;
	void SetLatitude(const Latitude& lati);

	virtual const Longitude& GetLongitude() const;
	void SetLongitude(const Longitude& longi);


	static Hent FromJson(const json::value& value);
	json::value ToJson() const;


protected:
	uint32_t m_Id;

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
	std::wstring m_extras;
	
	std::wstring m_plate;	
	HentType m_henttype;
	bool m_syncit;
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

	friend class SerializableHent;
};

//typedef boost::shared_ptr<Hent> HentObj;

#endif
