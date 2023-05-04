#include "stdafx.h"
#include "HentViewCreator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HentViewCreator::HentViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

void HentViewCreator::RefreshView(HentView* pView,HentClientObject* pHentCO)
{
	IHent* pHent = pHentCO->Object();
	pView->SetHentType(pHent->GetHentType());	
	pView->SetHentNo(pHent->GetHentNo());
	pView->SetAlias(pHent->GetAlias());
	pView->SetCity(pHent->GetCity());
	pView->SetExtras(pHent->GetExtras());
	pView->SetName(pHent->GetName());
	pView->SetFiscalNo(pHent->GetFiscalNo());
	pView->SetPhoneNo(pHent->GetPhoneNo());
	pView->SetPOBox(pHent->GetPOBox());
	pView->SetStreet(pHent->GetStreet());
	pView->SetWetNo(pHent->GetWetNo());
	pView->SetZip(pHent->GetZip());
	pView->SetPlateNo(pHent->GetPlateNo());			
	pView->SetPESEL(pHent->GetPESEL());
	pView->SetREGON(pHent->GetREGON());
	pView->SetIdNo(pHent->GetIdNo());
	pView->SetIssueDate(pHent->GetIssueDate());
	pView->SetIssuePost(pHent->GetIssuePost());
	pView->SetAccountNo(pHent->GetAccountNo());
	pView->SetBankName(pHent->GetBankName());
	pView->SetSyncIt(pHent->GetSyncIt());
	pView->SetWetLicenceNo(pHent->GetWetLicenceNo());
	pView->SetCellPhoneNo(pHent->GetCellPhoneNo());
	pView->SetEmailAddress(pHent->GetEmailAddress());
	pView->SetLatitude(pHent->GetLatitude());
	pView->SetLongitude(pHent->GetLongitude());
	pView->SetLocked(pHentCO->IsLocked());

}

HentView* HentViewCreator::CreateView(HentClientObject* pHentCO)
{
	HentView* pView = new HentView(pHentCO->Object()->GetId());			
	RefreshView(pView,pHentCO);
	return pView;			
}

void HentViewCreator::RefreshView(HentView* pView,const IInvoiceHent* pHent)
{
	
	pView->SetHentNo(pHent->GetHentNo());
	pView->SetCity(pHent->GetCity());
	pView->SetName(pHent->GetName());
	pView->SetFiscalNo(pHent->GetFiscalNo());
	pView->SetPOBox(pHent->GetPOBox());
	pView->SetStreet(pHent->GetStreet());
	pView->SetZip(pHent->GetZip());
	pView->SetPESEL(pHent->GetPESEL());
	pView->SetIdNo(pHent->GetIdNo());
	pView->SetIssueDate(pHent->GetIssueDate());
	pView->SetIssuePost(pHent->GetIssuePost());
	pView->SetAccountNo(pHent->GetAccountNo());
	pView->SetBankName(pHent->GetBankName());
	
}
