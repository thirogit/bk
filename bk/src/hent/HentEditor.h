#ifndef __HENTEDITOR_H__
#define __HENTEDITOR_H__

#include <string>
#include <data\impl\Hent.h>
#include "../context/SeasonSession.h"
#include <exception/TagException.h>

struct tag_HentEditorException{};

typedef TagException<tag_HentEditorException> HentEditorException;

class HentEditor
{
	Hent* m_pHent;
	SeasonSession* m_pSession;
	
public:
	HentEditor(SeasonSession* pSession);	
	~HentEditor();

	bool IsEditing() const;
	bool IsCreating() const;

	void Edit(uint32_t hentId);
	void New();
	void Save();

	uint32_t GetHentId() const;
	
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
	
	const std::wstring& GetPhoneNo() const;
	void SetPhoneNo(const std::wstring& sPhoneNo);
	
	const std::wstring& GetFiscalNo() const;
	void SetFiscalNo(const std::wstring& sFiscalNo);
	
	const HentNo& GetHentNo() const;
	void SetHentNo(const HentNo& hentNo);
	
	const std::wstring& GetWetNo() const;
	void SetWetNo(const std::wstring& sWetNo);	
	
	const std::wstring& GetExtras() const;
	void SetExtras(const std::wstring& sExtras);
		
	const std::wstring& GetPlateNo() const;	
	void SetPlateNo(const std::wstring& sPlate);
	
	HentType GetHentType() const;
	void SetHentType(HentType hentType);
	
	bool GetSyncIt() const;
	void SetSyncIt(bool bSyncIt);
		
	const std::wstring& GetPESEL() const;
	void SetPESEL(const std::wstring& sPESEL);
		
	const std::wstring& GetREGON() const;
	void SetREGON(const std::wstring& sREGON);
		
	const std::wstring& GetIdNo() const;
	void SetIdNo(const std::wstring& sIdNo);
		
	const DateTime& GetIssueDate() const;
	void SetIssueDate(const DateTime& IdIssueDate);
		
	const std::wstring& GetIssuePost() const;
	void SetIssuePost(const std::wstring& sIssuePost);

	const AccountNo& GetAccountNo() const;
	void SetAccountNo(const AccountNo& accountNo);
	
	const std::wstring& GetBankName() const;
	void SetBankName(const std::wstring& sBankName);

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
protected:
	void ThrowAlreadyStarted() const;
	void ThrowIfNotStarted() const;
	
};

#endif