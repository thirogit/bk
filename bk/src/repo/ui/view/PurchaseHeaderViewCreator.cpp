#include "stdafx.h"
#include "PurchaseHeaderViewCreator.h"

PurchaseHeaderViewCreator::PurchaseHeaderViewCreator()
{
}

PurchaseHeaderView* PurchaseHeaderViewCreator::CreateView(const IPurchaseHeader* pPurchaseHeader)
{
	PurchaseHeaderView* pView = new PurchaseHeaderView(pPurchaseHeader->GetId());
	RefreshView(pView, pPurchaseHeader);
	return pView;
}

void PurchaseHeaderViewCreator::RefreshView(PurchaseHeaderView* pView,const IPurchaseHeader* pPurchaseHeader)
{
	pView->SetAgent(pPurchaseHeader->GetAgentCode());	
	pView->SetCowAmount(pPurchaseHeader->GetCowCount());
	pView->SetDownloaded(pPurchaseHeader->GetWasDownloaded());
	pView->SetHerd(pPurchaseHeader->GetHerd());
	pView->SetPlateNo(pPurchaseHeader->GetPlateNo());
	pView->SetStartDt(pPurchaseHeader->GetStartDate());
}

