#include "stdafx.h"
#include "RecognitionItemViewCreator.h"
#include <utils/SafeGet.h>


RecognitionItemViewCreator::RecognitionItemViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

void RecognitionItemViewCreator::RefreshView(RecognitionItemView* pView, IRecognitionItem* pItem)
{
	
	CowDetails cow = pItem->GetCow();
	
	pView->SetCowNo(cow.GetCowNo());
	pView->SetPassNo(cow.GetPassNo());
				
	HentClientObject* pFistOwnerCO = m_pFacade->GetHent(cow.GetFirstOwnerId());
	IHent* pFirstOwner = SafeGet(&HentClientObject::Object,pFistOwnerCO,nullptr);
	pView->SetFirstOwnerAlias(SafeGet(&IHent::GetAlias,pFirstOwner,L""));

	pView->SetBirthDate(cow.GetBirthDate());
	pView->SetWeight(cow.GetWeight());
	pView->SetSex(cow.GetSex());

	IStock* pOwnStock = m_pFacade->GetStock(cow.GetStockId());
	pView->SetStockCd(SafeGet(&IStock::GetStockCode,pOwnStock,L""));

	IClass* pOwnClass = m_pFacade->GetClass(cow.GetClassId());
	pView->SetClassCd(SafeGet(&IClass::GetClassCode,pOwnClass,L""));

	pView->SetExtras(cow.GetExtras());

	
}

RecognitionItemView* RecognitionItemViewCreator::CreateView(IRecognitionItem* pItem)
{
	RecognitionItemView* pView = new RecognitionItemView(pItem->GetId());
	RefreshView(pView, pItem);
	return pView;			
}
