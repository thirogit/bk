#include "stdafx.h"
#include "BuyDocViewCreator.h"
#include <utils/SafeGet.h>
#include "../text/InvoiceNoText.h"

BuyDocViewCreator::BuyDocViewCreator(SeasonSession* facade) : m_pFacade(facade),m_docViewCreator(facade)
{
}

HentsDocView* BuyDocViewCreator::CreateView(IBuyDoc* pDoc)
{
	HentsDocView* pView = new HentsDocView(pDoc->GetId());

	RefreshView(pView,pDoc);
	
	return pView;

}

void BuyDocViewCreator::RefreshView(HentsDocView* pView,IBuyDoc* pDoc)
{
	m_docViewCreator.RefreshView(pView,pDoc);
	
	uint32_t invoiceId = pDoc->GetInvoiceId();

	IBuyInvoice* pInvoice = NULL;
	if(invoiceId != NULL_ID)
	{
		BuyInvoiceClientObject* pInvoiceCO = m_pFacade->GetBuyInvoice(invoiceId);
		pInvoice = SafeGet(&BuyInvoiceClientObject::Object,pInvoiceCO,nullptr);
	}

	pView->SetInvoiceNo(InvoiceNoText(pInvoice).ToString());
}