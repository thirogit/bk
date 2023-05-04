#include "stdafx.h"
#include "SellInvoiceViewCreator.h"
#include <utils/SafeGet.h>
#include "../text/InvoiceNoText.h"

SellInvoiceViewCreator::SellInvoiceViewCreator(SeasonSession* facade) : InvoiceViewCreator(facade)
{
}

void SellInvoiceViewCreator::RefreshView(InvoiceView* pView,IInvoice* pInvoice)
{
	InvoiceViewCreator::RefreshView(pView,pInvoice);

	IInvoice* pCorrected = NULL;
	uint32_t correctedId = pInvoice->GetCorrectFor();
	if(correctedId != NULL_ID)
	{
		SellInvoiceClientObject* pInvoiceCO = m_pFacade->GetSellInvoice(correctedId);
		pCorrected = SafeGet(&SellInvoiceClientObject::Object,pInvoiceCO,nullptr);
	}
		
	pView->SetCorrectFor(InvoiceNoText(pCorrected).ToString());
	
	
}

