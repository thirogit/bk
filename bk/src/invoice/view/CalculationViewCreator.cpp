#include "stdafx.h"
#include "CalculationViewCreator.h"
#include <utils\SafeGet.h>
#include "../../uiconstants/ColorConstants.h"

CalculationViewCreator::CalculationViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

CalculationElementView* CalculationViewCreator::CreateView(const CalculationElement* pElement)
{
	CalculationElementView* pView = new CalculationElementView(pElement->GetId());
	RefreshView(pElement,pView);
	return pView;
}

ICow* CalculationViewCreator::GetCow(const CalculationElement* pElement)
{
	const InvoiceTransactionItem* pItem = pElement->GetItem();
	CowClientObject* pCowCO = m_pFacade->GetCow(pItem->GetCowId());
	return pCowCO->Object();	
}

void CalculationViewCreator::RefreshView(const CalculationElement* pCalculationElement,CalculationElementView* pView)
{
	const InvoiceTransactionItem* pItem = pCalculationElement->GetItem();
	const CowDetails& details = pItem->GetCowDetails();
	const CowInvoiceEntryDetails& entry = pItem->GetEntry();
	const CowEntryDetails& docEntry = pItem->GetDocEntry();
		
	//ICow* pCow = GetCow(pCalculationElement);
			
	pView->SetOtherPrice(ColoredObject<NullDecimal>());

	HentClientObject* pOtherHentCO = m_pFacade->GetHent(GetOtherHent(pCalculationElement));
	IHent* pOtherHent = SafeGet(&HentClientObject::Object,pOtherHentCO,nullptr);
	pView->SetOtherHentAlias(SafeGet(&IHent::GetAlias,pOtherHent,L""));

	pView->SetOtherPrice(ColoredObject<NullDecimal>(GetOtherPrice(pCalculationElement)));

	pView->SetCowNo(details.GetCowNo());

	pView->SetHasTerminalValues(false);	

	uint32_t classId = pCalculationElement->GetClassId();
	IClass* pClass = m_pFacade->GetClass(classId);
	std::wstring classCd = pClass->GetClassCode();

	Color classClr;
	if(pCalculationElement->IsCustomClass())
		classClr = CUSTOM_SETING_CLR;

	pView->SetClass(ColoredObject<std::wstring>(classCd, classClr ));

	Color weightColor;

	if(pCalculationElement->IsCustomWeight())
		weightColor = CUSTOM_SETING_CLR;
	else
	{
		if(pCalculationElement->IsAvgWeight())
		{
			weightColor = AVERAGE_WEIGHT_CLR;
		}		
	}	

	ColoredObject<Decimal> weight(pCalculationElement->GetWeight(),weightColor);	
	pView->SetWeight(weight);	
		
	pView->SetRule(ColoredObject<std::wstring>());	

	Color netPricePerKgClr;
	Color grossPriceClr;
	Color netPriceClr;
	Color taxValueClr;
		
	CustomPrice customInd = pCalculationElement->GetCustomPrice();

	if(customInd == CustomPrice_Net || customInd == CustomPrice_Gross )					
	{
		netPricePerKgClr = CUSTOM_PRICE_CLR;
	}
	else if(customInd == CustomPrice_NetPerKg)
	{
		netPricePerKgClr = CUSTOM_SETING_CLR;
	} 
	else if(pCalculationElement->IsAvgPrice())
	{
		netPricePerKgClr = AVERAGE_PRICE_CLR;
	}
	else if(pCalculationElement->IsCustomPricePerKgForAll())
	{
		netPricePerKgClr = CUSTOM_PRICE_PER_KG_CLR;
	}
	//else if(m_pInvoiceSession->IsUseStockMyPerKgPrice())
	//{
	//	netPricePerKgClr = STOCK_PER_KG_PRICE_CLR;
	//}

	
	if(customInd == CustomPrice_Net)
	{
		netPriceClr = CUSTOM_SETING_CLR;
					
	}
	else if(customInd == CustomPrice_NetPerKg || customInd == CustomPrice_Gross)
	{
		netPriceClr = CUSTOM_PRICE_CLR;
	}
	else if(pCalculationElement->IsAvgPrice())
	{
		netPriceClr = AVERAGE_PRICE_CLR;
	}

		
	if(customInd == CustomPrice_Gross)
	{
		grossPriceClr = CUSTOM_SETING_CLR;
	}
	else if(customInd == CustomPrice_NetPerKg || customInd == CustomPrice_Net)
	{
		grossPriceClr = CUSTOM_PRICE_CLR;
	} 
	else if(pCalculationElement->IsAvgPrice())
	{
		grossPriceClr = AVERAGE_PRICE_CLR;
	}
			

	if(customInd != CustomPrice_None)
		taxValueClr = CUSTOM_PRICE_CLR;
	else if(pCalculationElement->IsAvgPrice())
		taxValueClr = AVERAGE_PRICE_CLR;
			


	ColoredObject<Decimal> netPricePerKg(pCalculationElement->GetNetPricePerKg(),netPricePerKgClr);
	pView->SetNetPricePerKg(netPricePerKg);

	ColoredObject<Decimal> netPrice(pCalculationElement->GetNetPrice(),netPriceClr);
	pView->SetNetPrice(netPrice);

	ColoredObject<Decimal> grossPrice(pCalculationElement->GetGrossPrice(),grossPriceClr);
	pView->SetGrossPrice(grossPrice);

	pView->SetTaxValue(ColoredObject<Decimal>(pCalculationElement->GetTaxValue(),taxValueClr));

	

	
}

