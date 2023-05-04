#include "stdafx.h"
#include "SellCalculationViewCreator.h"
#include <utils\SafeGet.h>
#include <data\utils\GetDocInvoice.h>

SellCalculationViewCreator::SellCalculationViewCreator(SeasonSession* facade) : CalculationViewCreator(facade)
{
}


NullDecimal SellCalculationViewCreator::GetOtherPrice(const CalculationElement* pElement)
{
	NullDecimal otherPrice;
	IBuyInvoice* pBuyInvoice = GetBuyInvoice(pElement);
	if(pBuyInvoice)
	{
		ICowInvoiceEntry* pEntry = pBuyInvoice->GetCow(pElement->GetItem()->GetCowId());
		otherPrice = pEntry->GetPrice();
	}	
	return otherPrice;
}

IBuyInvoice* SellCalculationViewCreator::GetBuyInvoice(const CalculationElement* pElement)
{
	ICow* pCow = GetCow(pElement);

	uint32_t invoiceId = pCow->GetBuyInvoiceId();
	if(invoiceId == NULL_ID)
	{
		ICowTrail* pTrail = m_pFacade->GetCowTrail(pCow->GetId());
		const ICowFootprint* pEntryFootprint = pTrail->GetEntry();
		invoiceId = GetDocInvoice(pEntryFootprint->GetDoc());
	}

	BuyInvoiceClientObject* pInvoiceCO = NULL;
	if(invoiceId != NULL_ID)
	{
		pInvoiceCO = m_pFacade->GetBuyInvoice(invoiceId);
	}

	return SafeGet(&BuyInvoiceClientObject::Object,pInvoiceCO,nullptr);
}

uint32_t SellCalculationViewCreator::GetOtherHent(const CalculationElement* pElement)
{
	const IInvoiceHent* pInvoiceHent = SafeGet(&IInvoice::GetInvoiceHent,(IInvoice*)GetBuyInvoice(pElement),nullptr);
	return (pInvoiceHent ? pInvoiceHent->GetParentHentId() : NULL_ID);
}

NullDecimal SellCalculationViewCreator::GetProfit(const CalculationElement* pElement)
{
	NullDecimal profit;

	NullDecimal buyPrice = GetOtherPrice(pElement);
	if(!buyPrice.IsNull())
		profit = NullDecimal(pElement->GetNetPrice()) - buyPrice;

	return profit;
}
