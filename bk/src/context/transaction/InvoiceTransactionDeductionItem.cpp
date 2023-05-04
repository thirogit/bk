#include "stdafx.h"
#include "InvoiceTransactionDeductionItem.h"


InvoiceTransactionDeductionItem::InvoiceTransactionDeductionItem(const ItemId& itemId) : m_id(itemId)
{
}


ItemId InvoiceTransactionDeductionItem::GetId() const
{
	return m_id;
}

const InvoiceDeduction& InvoiceTransactionDeductionItem::GetDeduction() const
{
	return m_deduction;
}

void InvoiceTransactionDeductionItem::SetDeduction(const InvoiceDeduction& deduction)
{
	m_deduction.CopyFrom(&deduction);
}

DeductionDetails InvoiceTransactionDeductionItem::GetDetails() const
{
	return m_deduction.GetDetails();
}

void InvoiceTransactionDeductionItem::SetDetails(const DeductionDetails& details)
{
	m_deduction.CopyFrom(details);
}

bool InvoiceTransactionDeductionItem::IsEnabled() const
{
	return m_deduction.GetIsEnabled();
}

void InvoiceTransactionDeductionItem::SetEnabled(bool bEnabled)
{
	m_deduction.SetIsEnabled(bEnabled);
}
