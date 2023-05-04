#include "stdafx.h"
#include "InvoiceCalculator.h"


namespace  InvoiceCalculator
{
	
	CalculationResult::CalculationResult()
	{
	}

	CalculationResult::CalculationResult(const CalculationResult& src)
	{
		operator=(src);
	}

	CalculationResult& CalculationResult::operator=(const CalculationResult& right)
	{
		m_grossValue = right.m_grossValue;
		m_netValue = right.m_netValue;
		m_taxValue = right.m_taxValue;
		m_weightValue = right.m_weightValue;
		m_deductedAmount = right.m_deductedAmount;
		return *this;
	}
	
	const Decimal& CalculationResult::GetGrossValue() const
	{
		return m_grossValue;
	}

	const Decimal& CalculationResult::GetNetValue() const
	{
		return m_netValue;
	}

	const Decimal& CalculationResult::GetTaxValue() const
	{
		return m_taxValue;
	}

	void CalculationResult::SetGrossValue(const Decimal& value)
	{
		m_grossValue = value;
	}

	void CalculationResult::SetNetValue(const Decimal& value)
	{
		m_netValue = value;
	}

	void CalculationResult::SetTaxValue(const Decimal& value)
	{
		m_taxValue = value;
	}

	void CalculationResult::SetWeight(const Decimal& value)
	{
		m_weightValue = value;
	}

	const Decimal& CalculationResult::GetDeductedAmount() const
	{
		return m_deductedAmount;
	}

	Decimal CalculationResult::GetGrossAfterDeductionsValue() const
	{
		return m_grossValue - m_deductedAmount;
	}

	const Decimal& CalculationResult::GetWeight() const
	{
		return m_weightValue;
	}

	void CalculationResult::SetDeductedAmount(const Decimal& amount)
	{
		m_deductedAmount = amount;
	}

	CalculationResult Calculate(const IInvoice* pInvoice)
	{
		Decimal totalNet(0L);
		Decimal totalWeight(0L);
		PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();
		ICowInvoiceEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;

			totalNet += pEntry->GetPrice();
			totalWeight += pEntry->GetWeight();

			entryEnum.advance();
		}

		Decimal deductedAmount;
		IInvoiceDeduction* deduction;
		PtrEnumerator<IInvoiceDeduction> deductionEnum = pInvoice->EnumDeductions();
		while (deductionEnum.hasNext())
		{
			deduction = *deductionEnum;
			if (deduction->GetIsEnabled())
			{
				deductedAmount += deduction->GetFraction()*totalNet;
			}
			deductionEnum.advance();
		}

		CalculationResult result;
		Decimal totalTax = totalNet*pInvoice->GetVATRate();
		Decimal grossValue = totalNet + totalTax;
		
		result.SetNetValue(totalNet);
		result.SetGrossValue(grossValue);
		result.SetTaxValue(totalTax);
		result.SetWeight(totalWeight);
		result.SetDeductedAmount(deductedAmount);


		return result;


	}
};

