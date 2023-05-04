#include "stdafx.h"
#include "BuyInvoiceViewCreator.h"
#include <utils/SafeGet.h>
#include "../text/InvoiceNoText.h"

BuyInvoiceViewCreator::BuyInvoiceViewCreator(SeasonSession* facade) : InvoiceViewCreator(facade)
{
}

void BuyInvoiceViewCreator::RefreshView(InvoiceView* pView,IInvoice* pInvoice)
{
	InvoiceViewCreator::RefreshView(pView,pInvoice);

	IInvoice* pCorrected = NULL;
	uint32_t correctedId = pInvoice->GetCorrectFor();
	if(correctedId != NULL_ID)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = m_pFacade->GetBuyInvoice(correctedId);
		pCorrected = SafeGet(&BuyInvoiceClientObject::Object,pBuyInvoiceCO,nullptr);
	}
		
	pView->SetCorrectFor(InvoiceNoText(pCorrected).ToString());
	
	
}

