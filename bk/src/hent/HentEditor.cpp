#include "stdafx.h"
#include "HentEditor.h"
#include "../settings/DefaultValuesEditor.h"

HentEditor::HentEditor(SeasonSession* pSession) : m_pHent(NULL), m_pSession(pSession)
{
}

HentEditor::~HentEditor()
{
	if(m_pHent)
		delete m_pHent;
}

bool HentEditor::IsEditing() const
{
	ThrowIfNotStarted();
	return m_pHent->GetId() != NULL_ID;
}

uint32_t HentEditor::GetHentId() const
{
	return m_pHent->GetId();
}

bool HentEditor::IsCreating() const
{
	return !IsEditing();
}

void HentEditor::ThrowIfNotStarted() const
{
	if(!m_pHent)
		throw HentEditorException(L"editor_not_started");
}

void HentEditor::ThrowAlreadyStarted() const
{
	if(m_pHent)
		throw HentEditorException(L"editor_already_started");
}

void HentEditor::Edit(uint32_t hentId)
{
	ThrowAlreadyStarted();

	m_pHent = new Hent(hentId);
	m_pHent->CopyFrom(m_pSession->GetHent(hentId)->Object());
	
}

void HentEditor::New()
{
	ThrowAlreadyStarted();

	m_pHent = new Hent(NULL_ID);

	DefaultValuesEditor editor(m_pSession->GetContext());
	DefaultValues defaultValues;
	editor.Load(defaultValues);
	if (defaultValues.bSetHentType)
	{
		m_pHent->SetHentType(defaultValues.henttype);
	}
}

void HentEditor::Save()
{
	ThrowIfNotStarted();
	
	if(m_pHent->GetId() != NULL_ID)
		m_pSession->UpdateHent(m_pHent);
	else
	{
		uint32_t newHentId = m_pSession->InsertHent(m_pHent);

		Hent* newHent = new Hent(newHentId);
		newHent->CopyFrom(m_pHent);
		delete m_pHent;
		m_pHent = newHent;
	}	
}	

const std::wstring& HentEditor::GetName() const
{
	ThrowIfNotStarted();
	return m_pHent->GetName();
}

void HentEditor::SetName(const std::wstring& sName)
{
	ThrowIfNotStarted();
	m_pHent->SetName(sName);
}

const std::wstring& HentEditor::GetAlias() const
{
	ThrowIfNotStarted();
	return m_pHent->GetAlias();
}

void HentEditor::SetAlias(const std::wstring& sAlias)
{
	ThrowIfNotStarted();
	m_pHent->SetAlias(sAlias);
}

const std::wstring& HentEditor::GetStreet() const
{
	ThrowIfNotStarted();
	return m_pHent->GetStreet();
}

void HentEditor::SetStreet(const std::wstring& sStreet)
{
	ThrowIfNotStarted();
	m_pHent->SetStreet(sStreet);
}
		
const std::wstring& HentEditor::GetPOBox() const
{
	ThrowIfNotStarted();
	return m_pHent->GetPOBox();
}

void HentEditor::SetPOBox(const std::wstring& sPOBox)
{
	ThrowIfNotStarted();
	m_pHent->SetPOBox(sPOBox);
}
	
const std::wstring& HentEditor::GetCity() const
{
	ThrowIfNotStarted();
	return m_pHent->GetCity();
}

void HentEditor::SetCity(const std::wstring& sCity)
{
	ThrowIfNotStarted();
	m_pHent->SetCity(sCity);
}
	
const std::wstring& HentEditor::GetZip() const
{
	ThrowIfNotStarted();
	return m_pHent->GetZip();
}

void HentEditor::SetZip(const std::wstring& sZip)
{
	ThrowIfNotStarted();
	m_pHent->SetZip(sZip);
}
	
const std::wstring& HentEditor::GetPhoneNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetPhoneNo();
}

void HentEditor::SetPhoneNo(const std::wstring& sPhoneNo)
{
	ThrowIfNotStarted();
	m_pHent->SetPhoneNo(sPhoneNo);
}
	
const std::wstring& HentEditor::GetFiscalNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetFiscalNo();
}

void HentEditor::SetFiscalNo(const std::wstring& sFiscalNo)
{
	ThrowIfNotStarted();
	m_pHent->SetFiscalNo(sFiscalNo);
}
	
const HentNo& HentEditor::GetHentNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetHentNo();
}

void HentEditor::SetHentNo(const HentNo& hentNo)
{
	ThrowIfNotStarted();
	m_pHent->SetHentNo(hentNo);
}
	
const std::wstring& HentEditor::GetWetNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetWetNo();
}

void HentEditor::SetWetNo(const std::wstring& sWetNo)
{
	ThrowIfNotStarted();
	m_pHent->SetWetNo(sWetNo);
}
	
const std::wstring& HentEditor::GetExtras() const
{
	ThrowIfNotStarted();
	return m_pHent->GetExtras();
}

void HentEditor::SetExtras(const std::wstring& sExtras)
{
	ThrowIfNotStarted();
	m_pHent->SetExtras(sExtras);
}
		
const std::wstring& HentEditor::GetPlateNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetPlateNo();
}

void HentEditor::SetPlateNo(const std::wstring& sPlate)
{
	ThrowIfNotStarted();
	m_pHent->SetPlateNo(sPlate);
}
	
HentType HentEditor::GetHentType() const
{
	ThrowIfNotStarted();
	return m_pHent->GetHentType();
}

void HentEditor::SetHentType(HentType hentType)
{
	ThrowIfNotStarted();
	m_pHent->SetHentType(hentType);
}
	
bool HentEditor::GetSyncIt() const
{
	ThrowIfNotStarted();
	return m_pHent->GetSyncIt();
}

void HentEditor::SetSyncIt(bool bSyncIt)
{
	ThrowIfNotStarted();
	m_pHent->SetSyncIt(bSyncIt);
}

const std::wstring& HentEditor::GetPESEL() const
{
	ThrowIfNotStarted();
	return m_pHent->GetPESEL();
}

void HentEditor::SetPESEL(const std::wstring& sPESEL)
{
	ThrowIfNotStarted();
	m_pHent->SetPESEL(sPESEL);
}
		
const std::wstring& HentEditor::GetREGON() const
{
	ThrowIfNotStarted();
	return m_pHent->GetREGON();
}

void HentEditor::SetREGON(const std::wstring& sREGON)
{
	ThrowIfNotStarted();
	m_pHent->SetREGON(sREGON);
}
		
const std::wstring& HentEditor::GetIdNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetIdNo();
}

void HentEditor::SetIdNo(const std::wstring& sIdNo)
{
	ThrowIfNotStarted();
	m_pHent->SetIdNo(sIdNo);
}
		
const DateTime& HentEditor::GetIssueDate() const
{
	ThrowIfNotStarted();
	return m_pHent->GetIssueDate();
}

void HentEditor::SetIssueDate(const DateTime& IdIssueDate)
{
	ThrowIfNotStarted();
	m_pHent->SetIssueDate(IdIssueDate);
}
		
const std::wstring& HentEditor::GetIssuePost() const
{
	ThrowIfNotStarted();
	return m_pHent->GetIssuePost();
}

void HentEditor::SetIssuePost(const std::wstring& sIssuePost)
{
	ThrowIfNotStarted();
	m_pHent->SetIssuePost(sIssuePost);
}

const AccountNo& HentEditor::GetAccountNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetAccountNo();
}

void HentEditor::SetAccountNo(const AccountNo& accountNo)
{
	ThrowIfNotStarted();
	m_pHent->SetAccountNo(accountNo);
}
	
const std::wstring& HentEditor::GetBankName() const
{
	ThrowIfNotStarted();
	return m_pHent->GetBankName();
}

void HentEditor::SetBankName(const std::wstring& sBankName)
{
	ThrowIfNotStarted();
	m_pHent->SetBankName(sBankName);
}

const std::wstring& HentEditor::GetWetLicenceNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetWetLicenceNo();
}

void HentEditor::SetWetLicenceNo(const std::wstring& sWetLicenceNo)
{
	ThrowIfNotStarted();
	m_pHent->SetWetLicenceNo(sWetLicenceNo);
}
	
const std::wstring& HentEditor::GetCellPhoneNo() const
{
	ThrowIfNotStarted();
	return m_pHent->GetCellPhoneNo();
}

void HentEditor::SetCellPhoneNo(const std::wstring& sCellPhoneNo)
{
	ThrowIfNotStarted();
	m_pHent->SetCellPhoneNo(sCellPhoneNo);
}

const std::wstring& HentEditor::GetEmailAddress() const
{
	ThrowIfNotStarted();
	return m_pHent->GetEmailAddress();
}

void HentEditor::SetEmailAddress(const std::wstring& sEmailAddress)
{
	ThrowIfNotStarted();
	m_pHent->SetEmailAddress(sEmailAddress);
}


const Latitude& HentEditor::GetLatitude() const
{
	ThrowIfNotStarted();
	return m_pHent->GetLatitude();
}

void HentEditor::SetLatitude(const Latitude& lati)
{
	ThrowIfNotStarted();
	m_pHent->SetLatitude(lati);
}

const Longitude& HentEditor::GetLongitude() const
{
	ThrowIfNotStarted();
	return m_pHent->GetLongitude();
}

void HentEditor::SetLongitude(const Longitude& longi)
{
	ThrowIfNotStarted();
	m_pHent->SetLongitude(longi);
}
	
