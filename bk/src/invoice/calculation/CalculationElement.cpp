#include "stdafx.h"
#include "CalculationElement.h"
#include "InvoiceCalculation.h"

CalculationElement::CalculationElement(const InvoiceTransactionItem *pItem, InvoiceCalculation *pCalculation) :
	m_pCalculation(pCalculation),m_pItem(pItem),m_customPrice(CustomPrice_None),m_customClassId(NULL_ID)
{
}

const InvoiceTransactionItem* CalculationElement::GetItem() const
{
	return m_pItem;
}

ItemId CalculationElement::GetId() const
{
	return m_pItem->GetItemKey().GetItemId();
}
		
Decimal CalculationElement::GetNetPrice() const
{
	Decimal netPrice;
	switch(m_customPrice)
	{
		case CustomPrice_None:

			if(m_pCalculation->IsAvgPriceEnabled()) 
			{
				netPrice = m_pCalculation->GetAvgPrice().ToDecimal(Decimal(0L));
				break;
			}
			netPrice = (GetWeight()*GetNetPricePerKg());
		break;

		case CustomPrice_NetPerKg:
			netPrice = (GetWeight()*GetNetPricePerKg());
		break;
		case CustomPrice_Net:
		case CustomPrice_Gross:
			netPrice =  m_customNetPrice.ToDecimal(Decimal(0L));
		break;
		
		default:
			ASSERT(0);
	}

	return netPrice;

}

Decimal CalculationElement::GetGrossPrice() const
{
	Decimal netto;
	netto = GetNetPrice();		
	return (netto + netto*m_pCalculation->GetVATRate() );
}

Decimal CalculationElement::GetTaxValue() const
{
	return GetNetPrice()*m_pCalculation->GetVATRate();
}
	
void CalculationElement::SetCustomWeight(const NullDecimal& weight)
{
	m_customWeight = weight;
}

NullDecimal CalculationElement::GetCustomWeight() const
{
	return m_customWeight;
}

bool CalculationElement::IsCustomWeight() const
{
	return !m_customWeight.IsNull();
}

void CalculationElement::SetCustomClass(uint32_t classId)
{
	if(classId == GetInheritedClass())
		m_customClassId = NULL_ID;
	else
		m_customClassId = classId;
}

uint32_t CalculationElement::GetCustomClass() const
{
	return m_customClassId;
}

bool CalculationElement::IsCustomClass() const
{
	return m_customClassId != NULL_ID;
}
	
Decimal CalculationElement::GetWeight() const
{
	if(!m_customWeight.IsNull())		
		return m_customWeight.ToDecimal(Decimal(0L));

	if(m_pCalculation->IsAvgWeightEnabled())
		return m_pCalculation->GetAvgWeight().ToDecimal(Decimal(0L));

	const CowEntryDetails& docEntry = m_pItem->GetDocEntry();
	NullDecimal docWeight = docEntry.GetWeight();

	if(!docWeight.IsNull())
		return docWeight.ToDecimal(Decimal(0L));

	return m_pItem->GetCowDetails().GetWeight();

}

uint32_t CalculationElement::GetInheritedClass() const
{
	uint32_t docClassId = m_pItem->GetDocEntry().GetClassId();
	if(docClassId == NULL_ID)
	{
		return m_pItem->GetCowDetails().GetClassId();
	}
	return docClassId;
}

uint32_t CalculationElement::GetClassId() const
{
	if(m_customClassId == NULL_ID)
	{
		return GetInheritedClass();	
	}
	return m_customClassId;
}

Decimal CalculationElement::GetNetPricePerKg() const
{
	Decimal weight = GetWeight();
	Decimal pricePerKg;	
	
	switch(m_customPrice)
	{
		case CustomPrice_NetPerKg:
			pricePerKg = m_customNetPricePerKg.ToDecimal(Decimal(0L));
		break;

		case CustomPrice_Gross:
		case CustomPrice_Net:
			if(weight > Decimal(0L))
				pricePerKg = m_customNetPrice.ToDecimal(Decimal(0L))/weight;
			else
				pricePerKg = 0L;
			break;
	default:
	
		if(m_pCalculation->IsAvgPriceEnabled())
		{
			pricePerKg = m_pCalculation->GetAvgPrice().ToDecimal(Decimal(0L))/GetWeight();
		} 
		else if(m_pCalculation->IsUseCustomPricePerKgForAll())
		{	
			pricePerKg = m_pCalculation->GetCustomPricePerKgForAll().ToDecimal(Decimal(0L));			
		}
		//else if(m_pCowPriceSession->IsUseStockMyPerKgPrice())
		//{
		//	pricePerKg = GetStock()->GetMyPerKgPrice();
		//}
		//else if((rule = GetPriceRule()))
		//{
		//	weightPricePerKg = GetCowPricePerKg(rule,weight,pCow->GetSex());	
		//}
		else 
		{
			pricePerKg = 0L;
		}
		break;

	}	
	return pricePerKg;
	
}

bool CalculationElement::IsAvgWeight() const
{
	return m_pCalculation->IsAvgWeightEnabled();
}

void CalculationElement::SetCustomPricePerKg(const Decimal &pricePerKg)
{		
	m_customNetPricePerKg = pricePerKg;
	m_customPrice =  CustomPrice_NetPerKg;	
}

void CalculationElement::SetCustomGrossPrice(const Decimal& grossPrice)
{
	m_customNetPrice = grossPrice /(Decimal(1L)+m_pCalculation->GetVATRate());
	m_customPrice = CustomPrice_Gross;
}

void CalculationElement::SetCustomNetPrice(const Decimal& netPrice)
{
	m_customNetPrice = netPrice;
	m_customPrice = CustomPrice_Net;
}

CustomPrice CalculationElement::GetCustomPrice() const
{
	return m_customPrice;
}

void CalculationElement::DisableCustomPrice()
{
	m_customPrice = CustomPrice_None;
}


bool CalculationElement::IsCustomPricePerKgForAll() const
{
	return m_pCalculation->IsUseCustomPricePerKgForAll();
}

bool CalculationElement::IsAvgPrice() const
{
	return m_pCalculation->IsAvgPriceEnabled();
}