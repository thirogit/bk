#include "stdafx.h"
#include "SellDocsListContent.h"
#include "../view/SellDocViewCreator.h"
#include "../context/object/SellDocClientObject.h"
#include "../uiconstants/ColorConstants.h"

void SellDocsListContent::CreateContent()
{	
	SellDocViewCreator viewCreator(m_pSession);
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(GetHerdId());
	PtrEnumerator<ISellDoc> selldocEnum = pRegistry->EnumSellDocs();
	while(selldocEnum.hasNext())
	{
		AddRow(new HentsDocRow(viewCreator.CreateView(*selldocEnum)));
		selldocEnum.advance();
	}	
}

void SellDocsListContent::OnHerdSellDocInserted(uint32_t docId)
{
	SellDocClientObject* pSellDocCO = m_pSession->GetSellDoc(docId);
	SellDoc* pSellDoc = pSellDocCO->Object();
	SellDocViewCreator viewCreator(m_pSession);
	AddRow(new HentsDocRow(viewCreator.CreateView(pSellDoc)));			
	NotifyCountChanged();
}

void SellDocsListContent::SetLocked(bool bLocked,uint32_t docId)
{
	HentsDocRow* pRow = m_Rows.find_row(docId);
	if(pRow)
		pRow->SetRowColor(bLocked ? Color(LOCKED_ROW_COLOR) : Color());
	NotifyContentUpdated();
}

void SellDocsListContent::OnHerdSellDocDeleted(uint32_t docId)
{
	RemoveRow(docId);
	NotifyCountChanged();
}

void SellDocsListContent::OnHerdSellDocUpdated(uint32_t docId)
{
	HentsDocRow* pRow = m_Rows.find_row(docId);
	
	HentsDocView* pView = pRow->Content();
	SellDocClientObject* pDocCO = m_pSession->GetSellDoc(pView->GetDocId());
	SellDoc* pDoc = pDocCO->Object();						
	SellDocViewCreator viewCreator(m_pSession);
	viewCreator.RefreshView(pView,pDoc);
				
	NotifyContentUpdated();
}

void SellDocsListContent::OnHerdSellDocLocked(uint32_t docId)
{
	SetLocked(true,docId);
}

void SellDocsListContent::OnHerdSellDocUnlocked(uint32_t docId)
{
	SetLocked(false,docId);
}