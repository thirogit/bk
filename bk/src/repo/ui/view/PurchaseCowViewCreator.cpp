#include "stdafx.h"
#include "PurchaseCowViewCreator.h"
#include <utils/SafeGet.h>

void PurchaseCowViewCreator::RefreshView(PurchaseCowView* pView, IPurchaseCow* pCow)
{
	pView->SetPrice(pCow->GetPrice());
	pView->SetWeight(pCow->GetWeight());
	pView->SetStockCd(pCow->GetStockCd());
	pView->SetClassCd(pCow->GetClassCd());
	pView->SetCowNo(pCow->GetCowNo());
	pView->SetMotherNo(pCow->GetMotherNo());
	pView->SetBirthDate(pCow->GetBirthDt());
	pView->SetPassNo(pCow->GetPassportNo());
	pView->SetHealthCertNo(pCow->GetHealthCertNo());
	pView->SetPassDate(pCow->GetPassportIssueDt());
	pView->SetBirthPlace(pCow->GetBirthPlace());
	pView->SetFirstOwnerNo(pCow->GetFirstOwner());
	pView->SetSex(pCow->GetSex());
}

PurchaseCowView* PurchaseCowViewCreator::CreateView(IPurchaseCow* pCow)
{
	PurchaseCowView* pView = new PurchaseCowView(pCow->GetId());
	RefreshView(pView,pCow);
	return pView;			
}
