#include "stdafx.h"
#include "DeductionElement.h"
#include "InvoiceCalculation.h"


DeductionElement::DeductionElement(const InvoiceTransactionDeductionItem *pDeduction, InvoiceCalculation *pCalculation) : m_pCalculation(pCalculation)
{
	m_id = pDeduction->GetId();
	m_deduction = pDeduction->GetDeduction();
}

ItemId DeductionElement::GetId() const
{
	return m_id;
}


const InvoiceDeduction& DeductionElement::GetDeduction() const
{
	return m_deduction;
}

void DeductionElement::SetDeduction(const InvoiceDeduction& deduction)
{
	m_deduction = deduction;
}

NullDecimal DeductionElement::GetDeductedAmount() const
{
	NullDecimal deductedAmount;
	if (m_deduction.GetIsEnabled())
	{
		deductedAmount = m_pCalculation->GetNetTotal()*m_deduction.GetFraction();
	}
	return deductedAmount;
}


