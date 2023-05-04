#include "stdafx.h"
#include "BuyCalculationViewCreator.h"
#include <utils\SafeGet.h>
#include <data\utils\GetDocInvoice.h>

BuyCalculationViewCreator::BuyCalculationViewCreator(SeasonSession* facade) : CalculationViewCreator(facade)
{
}

NullDecimal BuyCalculationViewCreator::GetOtherPrice(const CalculationElement* pElement)
{
	NullDecimal otherPrice;
	ISellInvoice* pSellInvoice = GetSellInvoice(pElement);
	if(pSellInvoice)
	{
		ICowInvoiceEntry* pEntry = pSellInvoice->GetCow(pElement->GetItem()->GetCowId());
		otherPrice = pEntry->GetPrice();
	}
	
	return otherPrice;
}

ISellInvoice* BuyCalculationViewCreator::GetSellInvoice(const CalculationElement* pElement)
{
	ICow* pCow = GetCow(pElement);

	uint32_t sellInvoiceId = pCow->GetSellInvoiceId();
	if(sellInvoiceId == NULL_ID)
	{
		ICowTrail* pTrail = m_pFacade->GetCowTrail(pCow->GetId());
		const ICowFootprint* pExitFootprint = pTrail->GetExit();
		if(pExitFootprint != NULL)
			sellInvoiceId = GetDocInvoice(pExitFootprint->GetDoc());
	}

	SellInvoiceClientObject* pSellInvoiceCO = NULL;
	if(sellInvoiceId != NULL_ID)
	{
		pSellInvoiceCO = m_pFacade->GetSellInvoice(sellInvoiceId);
	}

	return SafeGet(&SellInvoiceClientObject::Object,pSellInvoiceCO,nullptr);
}

uint32_t BuyCalculationViewCreator::GetOtherHent(const CalculationElement* pElement)
{
	const IInvoiceHent* pInvoiceHent = SafeGet(&IInvoice::GetInvoiceHent,(IInvoice*)GetSellInvoice(pElement),nullptr);
	return (pInvoiceHent ? pInvoiceHent->GetParentHentId() : NULL_ID);
}

NullDecimal BuyCalculationViewCreator::GetProfit(const CalculationElement* pElement)
{
	NullDecimal profit;

	NullDecimal sellPrice = GetOtherPrice(pElement);
	if(!sellPrice.IsNull())
		profit = sellPrice - NullDecimal(pElement->GetNetPrice());

	return profit;
}
