#include "stdafx.h"
#include "InvoiceCalculation.h"
#include <boost/foreach.hpp>
#include <errortoken\ErrorToken.h>
#include <boost/range/algorithm/count_if.hpp>
#include <arrays\DeepDelete.h>

InvoiceCalculation::InvoiceCalculation(InvoiceTransactionFacade* pTransaction) : m_pTransaction(pTransaction)
{
	m_pTransaction->AddObserver(this);
}

InvoiceCalculation::~InvoiceCalculation()
{
	m_pTransaction->RemoveObserver(this);
	DeepDelete(m_elements);
	DeepDelete(m_deductions);
}

void InvoiceCalculation::New()
{
	const InvoiceTransactionItem* pItem;
	for(int i = 0, count = m_pTransaction->GetItemCount();i < count;i++)
	{
		pItem = m_pTransaction->GetItemAt(i);
		CalculationElement* pElement = new CalculationElement(pItem,this);
		m_elements.add_item(pElement);
	}

	const InvoiceTransactionDeductionItem* pDeduction;
	for (int i = 0, count = m_pTransaction->GetDeductionCount(); i < count; i++)
	{
		pDeduction = m_pTransaction->GetDeductionAt(i);
		DeductionElement* deductionElement = new DeductionElement(pDeduction, this);
		m_deductions.add_item(deductionElement);
	}
}

void InvoiceCalculation::Edit()
{
	const InvoiceTransactionItem* pItem;
	for(int i = 0, count = m_pTransaction->GetItemCount();i < count;i++)
	{
		pItem = m_pTransaction->GetItemAt(i);
		CalculationElement* pElement = new CalculationElement(pItem,this);
		const CowInvoiceEntryDetails& entryDetails = pItem->GetEntry();
		
		pElement->SetCustomClass(entryDetails.GetClassId());
		pElement->SetCustomNetPrice(entryDetails.GetPrice());
		pElement->SetCustomWeight(NullDecimal(entryDetails.GetWeight()));

		m_elements.add_item(pElement);
	}

	const InvoiceTransactionDeductionItem* deductionItem;
	for (int i = 0, count = m_pTransaction->GetDeductionCount(); i < count; i++)
	{
		deductionItem = m_pTransaction->GetDeductionAt(i);
		DeductionElement* deductionElement = new DeductionElement(deductionItem, this);
		m_deductions.add_item(deductionElement);
	}
}

void InvoiceCalculation::SetTargetNetTotal(const NullDecimal& targetTotalNet)
{	
	if(!targetTotalNet.IsNull())
	{
		long woCustomPrice = m_elements.size() - CountWithCustomPrice();
		if(woCustomPrice == 0)
			throw CalculationException(ErrorToken(L"all_has_custom_price").str());

		Decimal sumNetPrice = SumNetCustomPrice();
		Decimal _targetTotalNet = targetTotalNet.ToDecimal(Decimal(0L));
		if(_targetTotalNet <= sumNetPrice) 
		{
			throw CalculationException(ErrorToken(L"target_net_price_too_low")(L"min_price",sumNetPrice.ToString(2)).str());			
		}
		
		m_AvgPrice = (_targetTotalNet-sumNetPrice)/Decimal(woCustomPrice);		
	}
	else
	{
		m_AvgPrice.MakeNull();
	}
	m_CustomNetTotal = targetTotalNet;
	NotifyAllChanged();

}

void InvoiceCalculation::ThrowIfAllHasCustomPrice()
{
	if(m_elements.size() == CountWithCustomPrice())
		throw CalculationException(ErrorToken(L"all_has_custom_price").str());
}


void InvoiceCalculation::SetTargetGrossTotal(const NullDecimal& targetTotalGross)
{
	if(!targetTotalGross.IsNull())
	{
		ThrowIfAllHasCustomPrice();
		long woCustomPrice = m_elements.size() - CountWithCustomPrice();
		
		Decimal sumGrossPrice = SumGrossCustomPrice();
		Decimal _targetTotalGross = targetTotalGross.ToDecimal(Decimal(0L));
		if(_targetTotalGross <= sumGrossPrice) 
		{
			throw CalculationException(ErrorToken(L"target_gross_price_too_low")(L"min_price",sumGrossPrice.ToString(2)).str());			
		}
		
		Decimal _vat = GetVATRate()+Decimal(1L);
		Decimal totalNet = (_targetTotalGross-sumGrossPrice)/_vat;
		m_AvgPrice = totalNet/Decimal(woCustomPrice);		
	}
	else
	{
		m_AvgPrice.MakeNull();
	}

	m_CustomGrossTotal = targetTotalGross;

	NotifyAllChanged();
}

void InvoiceCalculation::SetTargetTotalWeight(const NullDecimal& targetWeight)
{
	if(!targetWeight.IsNull() && targetWeight > NullDecimal(0L))
	{
		Decimal weight = targetWeight.ToDecimal(Decimal(0L));
		long nonCustomWeight = m_elements.size()-CountWithCustomWeight();
		if(nonCustomWeight > 0)
		{
			Decimal totalCustomWeight = SumCustomWeight();
			Decimal toDistribute = weight-totalCustomWeight;
			
			if(toDistribute > Decimal(0L))
			{
				m_AvgWeight = toDistribute/Decimal(nonCustomWeight);				
			}
			else				
			{
				Decimal minWeight = totalCustomWeight;// + Decimal(nonCustomWeight)*Decimal(0.001)

				throw CalculationException(ErrorToken(L"target_weight_to_low")(L"min_weight",minWeight.ToString(3)).str());
				//throw docelowa waga musi byc wieksza niz totalCustomWeight+ Decimal(nonCustomWeight)*Decimal(0.001)
			}
		}
		else
		{
			throw CalculationException(ErrorToken(L"all_has_custom_weight").str());
		}
	}
	else
	{		
		m_AvgWeight.MakeNull();
	}
	m_CustomTotalWeight = targetWeight;
	NotifyAllChanged();
}

Decimal InvoiceCalculation::SumGrossCustomPrice()
{
	Decimal sum(0L);
	BOOST_FOREACH(CalculationElement* pElement,m_elements)
	{
		if(pElement->GetCustomPrice() != CustomPrice_None)
			sum += pElement->GetGrossPrice(); 
	}
	return sum;
}

Decimal InvoiceCalculation::SumNetCustomPrice()
{
	Decimal sum(0L);
	BOOST_FOREACH(CalculationElement* pElement,m_elements)
	{
		if(pElement->GetCustomPrice() != CustomPrice_None)
			sum += pElement->GetNetPrice(); 
	}
	return sum;
}

Decimal InvoiceCalculation::SumCustomWeight()
{
	Decimal sum(0L);

	BOOST_FOREACH(CalculationElement* pElement,m_elements)
	{
		sum += pElement->GetCustomWeight().ToDecimal(Decimal(0L)); 
	}

	return sum;
}

long InvoiceCalculation::CountWithCustomPrice()
{
	return boost::range::count_if(m_elements,[](CalculationElement* pElement) -> bool
								{
									return pElement->GetCustomPrice() != CustomPrice_None;
								});
}

long InvoiceCalculation::CountWithCustomWeight()
{
	long customWeightCount = 0;

	BOOST_FOREACH(CalculationElement* pElement,m_elements)
	{
		if(pElement->IsCustomWeight())
		{
			customWeightCount++;			
		}
	}
	return customWeightCount;
}

void InvoiceCalculation::RecalculateAverageWeight()
{
	Decimal customWeightSum = SumCustomWeight();
	long nonCustomWeightCount = m_elements.size()-CountWithCustomWeight();
		
	if(nonCustomWeightCount > 0)
	{
		Decimal weightToDistribute = (m_CustomTotalWeight.ToDecimal(Decimal(0L))-customWeightSum);
		m_AvgWeight = weightToDistribute/Decimal(nonCustomWeightCount);
	}
	else
	{
		m_AvgWeight = Decimal(0L);
	}
}

Decimal InvoiceCalculation::GetWeightTotal()
{
	Decimal sumWeight;
	BOOST_FOREACH(CalculationElement* pElement,m_elements)
	{
		sumWeight += pElement->GetWeight();
	}
	return sumWeight;
}

void InvoiceCalculation::SetItemCustomWeight(ItemId id,const NullDecimal& weight)
{
	CalculationElement* pElement = FindElement(id);

	if(!m_CustomTotalWeight.IsNull())
	{
		if(weight.IsNull()) //ustawiamy wage na null'owa
		{
			if(pElement->IsCustomWeight())
			{
				//sprawdzic czy taka zmiana nie spowoduje ze waga srednia zejdzie ponizej 0.001 kg

				pElement->SetCustomWeight(weight);

				RecalculateAverageWeight();
				
				NotifyAllChanged();
				return;
			}
		}
		else // ustawiamy wage na nie null'owa
		{
			Decimal customWeightSum = SumCustomWeight();

			if(pElement->IsCustomWeight())
			{
				customWeightSum	-= pElement->GetCustomWeight().ToDecimal(Decimal(0L));
			}

			if(customWeightSum+weight.ToDecimal(Decimal(0L)) > m_CustomTotalWeight.ToDecimal(Decimal(0L)))
			{
				throw CalculationException(ErrorToken(L"exceeds_target_weight").str());
			}
			else
			{
				pElement->SetCustomWeight(weight);
				RecalculateAverageWeight();
				NotifyAllChanged();
			}
			return;
		}
	}	
	pElement->SetCustomWeight(weight);	
	NotifyElementChanged(pElement);
}

void InvoiceCalculation::SetVATRate(const Decimal& vatRate)
{
	m_VATRate = vatRate;
	NotifyAllChanged();
}

const Decimal& InvoiceCalculation::GetVATRate() const
{
	return m_VATRate;
}

void InvoiceCalculation::SetCustomNetPricePerKgForAll(const NullDecimal& netPricePerKg)
{
	m_CustomPricePerKg4All = netPricePerKg;
	NotifyAllChanged();
}

void InvoiceCalculation::SetItemCustomNetPricePerKg(ItemId id,const NullDecimal& netPricePerKg)
{
	CalculationElement* pElement = FindElement(id);

	if(netPricePerKg.IsNull())
	{
		pElement->DisableCustomPrice();
		//RecalculateAverage();
	}
	else
	{
		pElement->SetCustomPricePerKg(netPricePerKg.ToDecimal(Decimal(0L)));
	}
	NotifyElementChanged(pElement);
}

void InvoiceCalculation::SetItemCustomNetPrice(ItemId id,const NullDecimal& netPrice)
{
	CalculationElement* pElement = FindElement(id);

	if(netPrice.IsNull())
	{
		pElement->DisableCustomPrice();
		//RecalculateAverage();
	}
	else
	{
		pElement->SetCustomNetPrice(netPrice.ToDecimal(Decimal(0L)));
	}
	NotifyElementChanged(pElement);
}

CalculationElement* InvoiceCalculation::FindElement(ItemId id)
{
	return m_elements.find_item(id);
}

void InvoiceCalculation::SetItemCustomGrossPrice(ItemId id,const NullDecimal& grossPrice)
{
	CalculationElement* pElement = FindElement(id);

	if(grossPrice.IsNull())
	{
		pElement->DisableCustomPrice();
		//RecalculateAverage();
	}
	else
	{
		pElement->SetCustomGrossPrice(grossPrice.ToDecimal(Decimal(0L)));
	}

	NotifyElementChanged(pElement);
}

void InvoiceCalculation::NotifyElementChanged(const CalculationElement* pElement)
{
	VisitObservers(&CalculationObserver::ElementChanged,pElement);					
}

void InvoiceCalculation::NotifyAllChanged()
{
	BOOST_FOREACH(CalculationElement* pElement,m_elements)
	{
		VisitObservers(&CalculationObserver::ElementChanged,pElement);						
	}
}

size_t InvoiceCalculation::GetElementCount() const
{
	return m_elements.size();
}

CalculationElements InvoiceCalculation::Elements()
{
	return CalculationElements(m_elements);
}

bool InvoiceCalculation::IsAvgPriceEnabled() const
{
	return !m_AvgPrice.IsNull();
}

NullDecimal InvoiceCalculation::GetAvgPrice() const
{
	return m_AvgPrice;
}

bool InvoiceCalculation::IsUseCustomPricePerKgForAll() const
{
	return !m_CustomPricePerKg4All.IsNull();
}

NullDecimal InvoiceCalculation::GetCustomPricePerKgForAll() const
{
	return m_CustomPricePerKg4All;
}

bool InvoiceCalculation::IsAvgWeightEnabled() const
{
	return !m_CustomTotalWeight.IsNull();
}

NullDecimal InvoiceCalculation::GetAvgWeight() const
{
	return m_AvgWeight;
}

Decimal InvoiceCalculation::GetNetTotal()
{
	Decimal netTotal(0L);
	BOOST_FOREACH(CalculationElement* pElement,m_elements)
	{
		netTotal += pElement->GetNetPrice();
	}
	return netTotal;
}

Decimal InvoiceCalculation::GetGrossAfterDeductions()
{
	Decimal deductedAmount(0L);
	Decimal netTotal = GetNetTotal();
	BOOST_FOREACH(DeductionElement* deductionElement, m_deductions)
	{
		const InvoiceDeduction& deduction = deductionElement->GetDeduction();
		if (deduction.GetIsEnabled())
		{
			deductedAmount += netTotal*deduction.GetFraction();
		}
	}
	
	return netTotal*m_VATRate + netTotal - deductedAmount;
}

Decimal InvoiceCalculation::GetGrossTotal()
{
	Decimal netTotal = GetNetTotal();
	return netTotal*m_VATRate + netTotal;
}

Decimal InvoiceCalculation::GetTaxValue()
{
	Decimal netTotal = GetNetTotal();
	return netTotal*m_VATRate;
}

const CalculationElement* InvoiceCalculation::GetElementAt(int index)
{
	return m_elements.item_at(index);
}

void InvoiceCalculation::SetItemCustomClass(ItemId id,uint32_t classId)
{
	CalculationElement* pElement = FindElement(id);
	pElement->SetCustomClass(classId);	
	NotifyElementChanged(pElement);
}

void InvoiceCalculation::OnItemUpdated(ItemId id)
{
	const InvoiceTransactionItem* pItem = m_pTransaction->GetItem(id);
}

void InvoiceCalculation::OnItemAdded(ItemId id)
{
	const InvoiceTransactionItem* pItem = m_pTransaction->GetItem(id);
	CalculationElement* pElement = new CalculationElement(pItem,this);
	m_elements.add_item(pElement);
	NotifyElementAdded(id);
}

void InvoiceCalculation::OnItemDeleted(ItemId id)
{
	CalculationElement* pElement = m_elements.find_item(id);	
	if (pElement != NULL)
	{
		m_elements.remove_item(id);
		delete pElement;
		NotifyElementDeleted(id);
	}
}

void InvoiceCalculation::NotifyElementAdded(ItemId id)
{
	CalculationElement* pAddedElement = FindElement(id);
	VisitObservers(&CalculationObserver::ElementAdded,pAddedElement);
}

void InvoiceCalculation::NotifyElementDeleted(ItemId id)
{
	VisitObservers(&CalculationObserver::ElementDeleted,id);
}

void InvoiceCalculation::NotifyDeductionAdded(ItemId id)
{
	DeductionElement* pAddedDeduction = FindDeduction(id);
	VisitObservers(&CalculationObserver::DeductionAdded, pAddedDeduction);
}

void InvoiceCalculation::NotifyDeductionDeleted(ItemId id)
{
	VisitObservers(&CalculationObserver::DeductionDeleted, id);
}

void InvoiceCalculation::NotifyDeductionUpdated(ItemId id)
{
	DeductionElement* pChangedDeduction = FindDeduction(id);
	VisitObservers(&CalculationObserver::DeductionChanged, pChangedDeduction);
}

void InvoiceCalculation::DisableCustomPrice(ItemId id)
{
	CalculationElement* pElement = FindElement(id);

	pElement->DisableCustomPrice();
	
	NotifyElementChanged(pElement);
}


void InvoiceCalculation::OnDeductionUpdated(ItemId id)
{
	const InvoiceTransactionDeductionItem* pItem = m_pTransaction->GetDeduction(id);
	DeductionElement* pDeduction = m_deductions.find_item(id);
	pDeduction->SetDeduction(pItem->GetDeduction());
	NotifyDeductionUpdated(id);
}

void InvoiceCalculation::OnDeductionAdded(ItemId id)
{
	const InvoiceTransactionDeductionItem* pItem = m_pTransaction->GetDeduction(id);
	DeductionElement* pDeduction = new DeductionElement(pItem, this);
	m_deductions.add_item(pDeduction);
	NotifyDeductionAdded(id);

}

void InvoiceCalculation::OnDeductionDeleted(ItemId id)
{
	DeductionElement* pDeduction = m_deductions.find_item(id);
	if (pDeduction != NULL)
	{
		m_deductions.remove_item(id);
		delete pDeduction;
		NotifyDeductionDeleted(id);
	}

}

DeductionElements InvoiceCalculation::Deductions()
{
	return DeductionElements(m_deductions);
}


DeductionElement* InvoiceCalculation::FindDeduction(ItemId id)
{
	return m_deductions.find_item(id);
}

void InvoiceCalculation::EnableDeduction(ItemId deductionId, bool bEnable)
{
	DeductionElement* deductionElement = m_deductions.find_item(deductionId);
	InvoiceDeduction deduction = deductionElement->GetDeduction();
	deduction.SetIsEnabled(bEnable);
	deductionElement->SetDeduction(deduction);
	NotifyDeductionUpdated(deductionId);
}

InvoiceTransactionFacade* InvoiceCalculation::GetTransaction()
{
	return m_pTransaction;
}