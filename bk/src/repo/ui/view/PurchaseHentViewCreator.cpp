#include "stdafx.h"
#include "PurchaseHentViewCreator.h"

PurchaseHentViewCreator::PurchaseHentViewCreator()
{
}

PurchaseHentView* PurchaseHentViewCreator::CreateView(const IPurchaseHent* hent)
{
	PurchaseHentView* pView = new PurchaseHentView(hent->GetId());
	RefreshView(pView, hent);
	return pView;
}

void PurchaseHentViewCreator::RefreshView(PurchaseHentView* pView, const IPurchaseHent* hent)
{
	pView->SetHentType(hent->GetHentType());
	pView->SetHentNo(hent->GetHentNo());
	pView->SetAlias(hent->GetAlias());
	pView->SetCity(hent->GetCity());
	pView->SetName(hent->GetName());
	pView->SetFiscalNo(hent->GetFiscalNo());
	pView->SetPhoneNo(hent->GetPhoneNo());
	pView->SetPOBox(hent->GetPOBox());
	pView->SetStreet(hent->GetStreet());
	pView->SetWetNo(hent->GetWetNo());
	pView->SetZip(hent->GetZip());
	pView->SetPlateNo(hent->GetPlateNo());
	pView->SetPersonalNo(hent->GetPersonalNo());
	pView->SetStatsNo(hent->GetStatsNo());
	pView->SetIdNo(hent->GetPersonalIdNo());
	pView->SetIssueDate(hent->GetIssueDate());
	pView->SetIssuePost(hent->GetIssuePost());
	pView->SetAccountNo(hent->GetAccountNo());
	pView->SetBankName(hent->GetBankName());
	pView->SetWetLicenceNo(hent->GetWetLicenceNo());
	pView->SetCellPhoneNo(hent->GetCellPhoneNo());
	pView->SetEmailAddress(hent->GetEmailAddress());
	pView->SetLatitude(hent->GetLatitude());
	pView->SetLongitude(hent->GetLongitude());
}

