#include "stdafx.h"
#include "CowViewCreator.h"
#include <utils/SafeGet.h>
#include "../text/DocNoText.h"

CowViewCreator::CowViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

void CowViewCreator::RefreshView(CowView* pView,ICow* pCow)
{
	uint32_t cowId = pView->GetId();
	ICowTrail* pTrail = m_pFacade->GetCowTrail(cowId);
	
	pView->SetCowNo(pCow->GetCowNo());
	pView->SetPassNo(pCow->GetPassNo());
				
	HentClientObject* pFistOwnerCO = m_pFacade->GetHent(pCow->GetFirstOwnerId());
	IHent* pFirstOwner = SafeGet(&HentClientObject::Object,pFistOwnerCO,nullptr);
	pView->SetFirstOwnerAlias(SafeGet(&IHent::GetAlias,pFirstOwner,L""));

	pView->SetBirthDate(pCow->GetBirthDate());
	pView->SetWeight(pCow->GetWeight());
	pView->SetSex(pCow->GetSex());

	IStock* pOwnStock = m_pFacade->GetStock(pCow->GetStockId());
	pView->SetStockCd(SafeGet(&IStock::GetStockCode,pOwnStock,L""));

	IClass* pOwnClass = m_pFacade->GetClass(pCow->GetClassId());
	pView->SetClassCd(SafeGet(&IClass::GetClassCode,pOwnClass,L""));

	pView->SetExtras(pCow->GetExtras());

	IClass* pTermBuyClass = m_pFacade->GetClass(pCow->GetTermBuyClassId());
	pView->SetTermBuyClassCd(SafeGet(&IClass::GetClassCode,pTermBuyClass,L""));

	IClass* pTermSellClass = m_pFacade->GetClass(pCow->GetTermSellClassId());
	pView->SetTermSellClassCd(SafeGet(&IClass::GetClassCode,pTermSellClass,L""));

	pView->SetTermBuyWeight(pCow->GetTermBuyWeight());
	pView->SetTermSellWeight(pCow->GetTermSellWeight());
	pView->SetTermBuyPrice(pCow->GetTermBuyPrice());
	pView->SetTermSellPrice(pCow->GetTermSellPrice());
}

CowView* CowViewCreator::CreateView(ICow* pCow)
{
	CowView* pView = new CowView(pCow->GetId());
	RefreshView(pView,pCow);
	return pView;			
}
