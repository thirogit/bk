#include "stdafx.h"
#include "InvoiceItemViewCreator.h"
#include <utils/SafeGet.h>
#include "../uiconstants/ColorConstants.h"
#include <utils/iif.h>

//InvoiceItemViewCreator::InvoiceItemViewCreator(SeasonSessionFacade* facade) : m_pFacade(facade)
//{
//}
//
//InvoiceItemView* InvoiceItemViewCreator::CreateView(const InvoiceItem* pItem)
//{
//	InvoiceItemView* pView = new InvoiceItemView(pItem->GetItemKey().GetItemId());
//	RefreshView(pItem,pView);
//	return pView;
//}
//
//void InvoiceItemViewCreator::RefreshView(const InvoiceItem* pItem,InvoiceItemView* pView)
//{
//	const CowDetails& details = pItem->GetCowDetails();
//	const CowInvoiceEntryDetails& entry = pItem->GetEntry();
//
//	//pView->SetCowNo(details.GetCowNo());	
//	//pView->SetPassNo(details.GetPassNo());
//	//HentClientObject* pFistOwnerCO = m_pFacade->GetHent(details.GetFirstOwnerId());
//	//IHent* pFirstOwner = SafeGet(&HentClientObject::Object,pFistOwnerCO,nullptr);
//	//pView->SetFstOwnerAlias(SafeGet(&IHent::GetAlias,pFirstOwner,""));
//	//pView->SetBirthDate(details.GetBirthDate());
//	//pView->SetSex(details.GetSex());
//
//	//Decimal weight = iif<const Decimal>(!entry.GetWeight().IsNull(),entry.GetWeight().ToDecimal(Decimal(0L)),details.GetWeight());
//	//pView->SetWeight(ColoredObject<Decimal>(weight,
//	//										iif(details.GetWeight() != weight,Color(CUSTOM_SETING_CLR),Color())));
//	//
//	//uint32_t stockId = (entry.GetStockId() != NULL_ID ? entry.GetStockId() : details.GetStockId());
//	//IStock * stock = m_pFacade->GetStock(stockId);
//
//	//pView->SetStock(ColoredObject<std::string>(SafeGet(&IStock::GetStockCode,stock,""),
//	//											iif(details.GetStockId() != stockId,Color(CUSTOM_SETING_CLR),Color())));
//
//	//uint32_t classId  = (entry.GetClassId() != NULL_ID ? entry.GetClassId() : details.GetClassId());
//	//IClass * pClass = m_pFacade->GetClass(classId);
//
//	//pView->SetClass(ColoredObject<std::string>(SafeGet(&IClass::GetClassCode,pClass,""),
//	//											iif(details.GetClassId() != classId,Color(CUSTOM_SETING_CLR),Color())));
//	//pView->SetGroup(ColoredObject<NullInt>(entry.GetGroup()));	
//}
