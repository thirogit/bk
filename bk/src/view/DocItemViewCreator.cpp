#include "stdafx.h"
#include "DocItemViewCreator.h"
#include <utils/SafeGet.h>
#include "../uiconstants/ColorConstants.h"
#include <utils/iif.h>

DocItemViewCreator::DocItemViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

DocItemView* DocItemViewCreator::CreateView(const DocItem* pItem)
{
	DocItemView* pView = new DocItemView(pItem->GetItemId());
	RefreshView(pItem,pView);
	return pView;
}

void DocItemViewCreator::RefreshView(const DocItem* pItem,DocItemView* pView)
{
	const CowDetails& details = pItem->GetCowDetails();
	const CowEntryDetails& entry = pItem->GetCowEntry();

	pView->SetCowNo(details.GetCowNo());	
	pView->SetPassNo(details.GetPassNo());
	HentClientObject* pFistOwnerCO = m_pFacade->GetHent(details.GetFirstOwnerId());
	IHent* pFirstOwner = SafeGet(&HentClientObject::Object,pFistOwnerCO,nullptr);
	pView->SetFstOwnerAlias(SafeGet(&IHent::GetAlias,pFirstOwner,L""));
	pView->SetBirthDate(details.GetBirthDate());
	pView->SetSex(details.GetSex());

	Decimal weight = iif<const Decimal>(!entry.GetWeight().IsNull(),entry.GetWeight().ToDecimal(Decimal(0L)),details.GetWeight());
	pView->SetWeight(ColoredObject<Decimal>(weight,
											iif(details.GetWeight() != weight,Color(CUSTOM_SETING_CLR),Color())));
	
	uint32_t stockId = (entry.GetStockId() != NULL_ID ? entry.GetStockId() : details.GetStockId());
	IStock * stock = m_pFacade->GetStock(stockId);

	pView->SetStock(ColoredObject<std::wstring>(SafeGet(&IStock::GetStockCode,stock,L""),
												iif(details.GetStockId() != stockId,Color(CUSTOM_SETING_CLR),Color())));

	uint32_t classId  = (entry.GetClassId() != NULL_ID ? entry.GetClassId() : details.GetClassId());
	IClass * pClass = m_pFacade->GetClass(classId);

	pView->SetClass(ColoredObject<std::wstring>(SafeGet(&IClass::GetClassCode,pClass,L""),
												iif(details.GetClassId() != classId,Color(CUSTOM_SETING_CLR),Color())));
	pView->SetGroup(ColoredObject<NullInt>(entry.GetGroup()));	
}
