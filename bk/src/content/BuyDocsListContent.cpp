#include "stdafx.h"
#include "BuyDocsListContent.h"
#include "../view/BuyDocViewCreator.h"
#include "../context/object/BuyDocClientObject.h"
#include "../uiconstants/ColorConstants.h"

void BuyDocsListContent::CreateContent()
{	
	BuyDocViewCreator viewCreator(m_pSession);
	HerdRegistry* pRegistry = m_pSession->GetHerdRegistry(GetHerdId());
	PtrEnumerator<IBuyDoc> buydocEnum = pRegistry->EnumBuyDocs();
	while(buydocEnum.hasNext())
	{
		IBuyDoc* pBuyDoc = *buydocEnum;
		BuyDocClientObject* pBuyDocCO = m_pSession->GetBuyDoc(pBuyDoc->GetId());
		HentsDocRow* pRow = new HentsDocRow(viewCreator.CreateView(pBuyDoc));
		pRow->SetRowColor(pBuyDocCO->IsLocked() ? Color(LOCKED_ROW_COLOR) : Color());
		AddRow(pRow);
		buydocEnum.advance();
	}	
}

void BuyDocsListContent::OnHerdBuyDocInserted(uint32_t docId)
{
	
	BuyDocClientObject* pBuyDocCO = m_pSession->GetBuyDoc(docId);
	BuyDoc* pBuyDoc = pBuyDocCO->Object();
	BuyDocViewCreator viewCreator(m_pSession);
	AddRow(new HentsDocRow(viewCreator.CreateView(pBuyDoc)));					
				
	NotifyCountChanged();
}

void BuyDocsListContent::OnHerdBuyDocUpdated(uint32_t docId)
{
	HentsDocRow* pRow = m_Rows.find_row(docId);
	
	HentsDocView* pView = pRow->Content();
	BuyDocClientObject* pDocCO = m_pSession->GetBuyDoc(pView->GetDocId());
	BuyDoc* pDoc = pDocCO->Object();						
	BuyDocViewCreator viewCreator(m_pSession);
	viewCreator.RefreshView(pView,pDoc);
				
	NotifyContentUpdated();

}

void BuyDocsListContent::SetLocked(bool bLocked,uint32_t docId)
{
	HentsDocRow* pRow = m_Rows.find_row(docId);
	if(pRow)
		pRow->SetRowColor(bLocked ? Color(LOCKED_ROW_COLOR) : Color());
	NotifyContentUpdated();
}

void BuyDocsListContent::OnHerdBuyDocLocked(uint32_t docId)
{
	SetLocked(true,docId);
}

void BuyDocsListContent::OnHerdBuyDocUnlocked(uint32_t docId)
{
	SetLocked(false,docId);
}

void BuyDocsListContent::OnHerdBuyDocDeleted(uint32_t docId)
{
	HentsDocRow* pRemovedRow = m_Rows.remove_row(docId);
	delete pRemovedRow;
	NotifyCountChanged();
}

