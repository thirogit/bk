#include "stdafx.h"
#include "SellDocViewCreator.h"
#include <utils/SafeGet.h>
#include "../text/InvoiceNoText.h"

SellDocViewCreator::SellDocViewCreator(SeasonSession* facade) : m_pFacade(facade),m_docViewCreator(facade)
{
}

HentsDocView* SellDocViewCreator::CreateView(ISellDoc* pDoc)
{
	HentsDocView* pView = new HentsDocView(pDoc->GetId());

	RefreshView(pView,pDoc);
	
	return pView;

}

void SellDocViewCreator::RefreshView(HentsDocView* pView,ISellDoc* pDoc)
{
	m_docViewCreator.RefreshView(pView,pDoc);
	
	uint32_t invoiceId = pDoc->GetInvoiceId();

	ISellInvoice* pInvoice = NULL;
	if(invoiceId != NULL_ID)
	{
		SellInvoiceClientObject* pInvoiceCO = m_pFacade->GetSellInvoice(invoiceId);
		pInvoice = SafeGet(&SellInvoiceClientObject::Object,pInvoiceCO,nullptr);
	}

	pView->SetInvoiceNo(InvoiceNoText(pInvoice).ToString());
}